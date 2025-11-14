#include <SFML/Graphics.h>

#include "Grid.h"
#define FONT_FILE "assets/font.ttf"

// Internal global font used for all sfText so we don't need one font per cell
static sfFont* g_font = NULL;

// Helper: check in bounds
static bool InBounds(int x, int y)
{
    return (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE);
}

// Create a new Cell
Cell* CellCreate(sfVector2f size, sfVector2f pos, sfColor color)
{
    Cell* newCell = (Cell*)malloc(sizeof(Cell));
    if (!newCell)
        return NULL;

    newCell->shape = sfRectangleShape_create();
    if (!newCell->shape)
    {
        free(newCell);
        return NULL;
    }
    sfRectangleShape_setSize(newCell->shape, size);
    sfRectangleShape_setPosition(newCell->shape, pos);
    sfRectangleShape_setFillColor(newCell->shape, color);

    // text
    newCell->text = sfText_create();
    if (!newCell->text)
    {
        sfRectangleShape_destroy(newCell->shape);
        free(newCell);
        return NULL;
    }
    if (g_font)
        sfText_setFont(newCell->text, g_font);
    sfText_setCharacterSize(newCell->text, 20);
    // center text roughly
    sfText_setString(newCell->text, "");
    newCell->bDiscovered = false;
    newCell->bFlagged = false;
    newCell->bPlanted = false;
    newCell->explosiveNeighbor = 0;

    return newCell;
}

// Draw a single cell
void CellDraw(Cell* cell, sfRenderWindow* window)
{
    if (!cell || !window) return;

    // draw rectangle
    sfRenderWindow_drawRectangleShape(window, cell->shape, NULL);

    // draw text if discovered or if flagged
    if (cell->bFlagged)
    {
        // flag text is handled via sfText (we set "F")
        sfRenderWindow_drawText(window, cell->text, NULL);
    }
    else if (cell->bDiscovered)
    {
        // if bomb -> draw "B"
        const char* s = sfText_getString(cell->text);
        if (s && strlen(s) > 0)
        {
            sfRenderWindow_drawText(window, cell->text, NULL);
        }
    }
}

// Reveal a cell. Returns FAILURE if bomb exploded, SUCCESS if game won (optional), 0 otherwise.
int CellReveal(Grid* grid, sfVector2i cellGridPos)
{
    if (!grid) return 0;
    int gx = cellGridPos.x;
    int gy = cellGridPos.y;
    if (!InBounds(gx, gy)) return 0;

    Cell* cell = grid->cells[gx][gy];
    if (!cell) return 0;

    if (cell->bDiscovered || cell->bFlagged)
        return 0;

    // If it's planted with a bomb -> explosion
    if (cell->bPlanted)
    {
        // reveal bomb
        cell->bDiscovered = true;
        sfText_setString(cell->text, "B");
        // mark cell color to indicate bomb
        sfRectangleShape_setFillColor(cell->shape, sfColor_fromRGB(200, 50, 50));
        return FAILURE;
    }

    // reveal the cell
    cell->bDiscovered = true;
    grid->discoveredCellCount++;

    // change appearance: lighter color
    sfRectangleShape_setFillColor(cell->shape, sfColor_fromRGB(200, 200, 200));

    // if has neighbors > 0 show number
    if (cell->explosiveNeighbor > 0)
    {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", cell->explosiveNeighbor);
        sfText_setString(cell->text, buf);

        // position text centered in cell
        sfFloatRect tb = sfText_getLocalBounds(cell->text);
        sfVector2f pos = sfRectangleShape_getPosition(cell->shape);
        sfVector2f size = sfRectangleShape_getSize(cell->shape);
        sfText_setPosition(cell->text, (sfVector2f) { pos.x + (size.x - tb.width) / 2.f - tb.left, pos.y + (size.y - tb.height) / 2.f - tb.top });
    }
    else
    {
        // empty cell -> flood fill neighbors
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0) continue;
                int nx = gx + dx;
                int ny = gy + dy;
                if (InBounds(nx, ny))
                {
                    Cell* ncell = grid->cells[nx][ny];
                    if (ncell && !ncell->bDiscovered && !ncell->bFlagged && !ncell->bPlanted)
                    {
                        // recursive reveal
                        CellReveal(grid, (sfVector2i) { nx, ny });
                    }
                }
            }
        }
    }

    // If all safe cells discovered, return SUCCESS
    int totalSafe = GRID_SIZE * GRID_SIZE - BOMB_COUNT;
    if (grid->discoveredCellCount >= totalSafe)
    {
        return SUCCESS;
    }

    return 0;
}

// Toggle flag on a cell
void CellFlag(Grid* grid, sfVector2i cellGridPos)
{
    if (!grid) return;
    int gx = cellGridPos.x;
    int gy = cellGridPos.y;
    if (!InBounds(gx, gy)) return;

    Cell* cell = grid->cells[gx][gy];
    if (!cell) return;

    if (cell->bDiscovered)
        return; // cannot flag discovered

    cell->bFlagged = !cell->bFlagged;
    if (cell->bFlagged)
    {
        sfText_setString(cell->text, "F");
        // center flag text
        sfFloatRect tb = sfText_getLocalBounds(cell->text);
        sfVector2f pos = sfRectangleShape_getPosition(cell->shape);
        sfVector2f size = sfRectangleShape_getSize(cell->shape);
        sfText_setPosition(cell->text, (sfVector2f) { pos.x + (size.x - tb.width) / 2.f - tb.left, pos.y + (size.y - tb.height) / 2.f - tb.top });
    }
    else
    {
        sfText_setString(cell->text, "");
    }
}

// Destroy a cell
void CellDestroy(Cell* cell)
{
    if (!cell) return;
    if (cell->text) sfText_destroy(cell->text);
    if (cell->shape) sfRectangleShape_destroy(cell->shape);
    free(cell);
}

// Create and initialize the entire grid
Grid* GridCreate()
{
    // load font
    g_font = sfFont_createFromFile(FONT_FILE);
    if (!g_font)
    {
        fprintf(stderr, "Failed to load font at '%s'\n", FONT_FILE);
        // continue but text won't be shown
    }

    Grid* newGrid = (Grid*)malloc(sizeof(Grid));
    if (!newGrid) return NULL;
    newGrid->discoveredCellCount = 0;

    // compute cell size and initial offset using constants
    int gridSize = GRID_SIZE;
    float cellSizeF = (float)CELL_SIZE;
    float offsetX = GRID_OFFSET;
    float offsetY = GRID_OFFSET;

    for (int x = 0; x < gridSize; ++x)
    {
        for (int y = 0; y < gridSize; ++y)
        {
            sfVector2f size = { cellSizeF - (float)CELL_OFFSET, cellSizeF - (float)CELL_OFFSET };
            sfVector2f pos = { offsetX + x * cellSizeF + CELL_OFFSET / 2.f, offsetY + y * cellSizeF + CELL_OFFSET / 2.f };
            sfColor color = sfColor_fromRGB(120, 120, 120);
            Cell* c = CellCreate(size, pos, color);
            newGrid->cells[x][y] = c;
        }
    }

    return newGrid;
}

// Plant bombs randomly, avoiding cellToAvoid
void GridPlantBomb(Grid* grid, int bombCount, sfVector2i cellToAvoid)
{
    if (!grid) return;

    int placed = 0;
    int totalCells = GRID_SIZE * GRID_SIZE;
    if (bombCount > totalCells - 1) bombCount = totalCells - 1;

    while (placed < bombCount)
    {
        int rx = rand() % GRID_SIZE;
        int ry = rand() % GRID_SIZE;

        // avoid specified cell
        if (rx == cellToAvoid.x && ry == cellToAvoid.y) continue;

        Cell* c = grid->cells[rx][ry];
        if (!c) continue;
        if (c->bPlanted) continue;

        c->bPlanted = true;
        placed++;

        // update neighbors' explosiveNeighbor count
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                int nx = rx + dx;
                int ny = ry + dy;
                if (InBounds(nx, ny) && !(dx == 0 && dy == 0))
                {
                    Cell* n = grid->cells[nx][ny];
                    if (n) n->explosiveNeighbor++;
                }
            }
        }
    }
}

// Return hovered cell coordinates or (-1,-1) if none
sfVector2i GridUpdateLoop(Grid* grid, sfRenderWindow* window)
{
    sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
    sfVector2i cellCoord = { -1, -1 };

    for (int x = 0; x < GRID_SIZE; ++x)
    {
        for (int y = 0; y < GRID_SIZE; ++y)
        {
            Cell* c = grid->cells[x][y];
            if (!c) continue;
            sfFloatRect bounds = sfRectangleShape_getGlobalBounds(c->shape);
            if (sfFloatRect_contains(&bounds, (float)mousePos.x, (float)mousePos.y))
            {
                cellCoord.x = x;
                cellCoord.y = y;
                return cellCoord;
            }
        }
    }

    return cellCoord;
}

// Draw the entire grid
void GridDraw(Grid* grid, sfRenderWindow* window)
{
    if (!grid || !window) return;

    for (int x = 0; x < GRID_SIZE; ++x)
    {
        for (int y = 0; y < GRID_SIZE; ++y)
        {
            Cell* c = grid->cells[x][y];
            if (!c) continue;
            CellDraw(c, window);
        }
    }
}

// Destroy grid and free resources
void GridDestroy(Grid* grid)
{
    if (!grid) return;

    for (int x = 0; x < GRID_SIZE; ++x)
    {
        for (int y = 0; y < GRID_SIZE; ++y)
        {
            Cell* c = grid->cells[x][y];
            if (c) CellDestroy(c);
            grid->cells[x][y] = NULL;
        }
    }

    if (g_font)
    {
        sfFont_destroy(g_font);
        g_font = NULL;
    }

    free(grid);
}