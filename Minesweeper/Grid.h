#pragma once

#include <SFML/System.h>

#include "basics.h"

/// <summary>
/// This struct represent a single cell in the Minesweeper grid.
/// All the properties related to a cell are defined here.
/// </summary>
typedef struct Cell {
	sfRectangleShape* shape;
	sfText* text;
	bool bDiscovered;
	bool bFlagged;
	bool bPlanted;
	int explosiveNeighbor;
} Cell;

/// <summary>
/// This struct represent the entire grid of the Minesweeper game.
/// It contains a 2D array of Cell pointers and a count of discovered cells.
/// </summary>
typedef struct Grid {
	Cell* cells[GRID_SIZE][GRID_SIZE];
	int discoveredCellCount;
} Grid;

/// <summary>
/// This function creates a new Cell with the specified size, position, and color.
/// </summary>
/// <param name="size">The size of the cell in sfVector2f format ({ width, height })</param>
/// <param name="pos">The position of the cell in sfVector2f format ({ x, y })</param>
/// <param name="color">The color of the cell in sfColor format</param>
Cell* CellCreate(sfVector2f size, sfVector2f pos, sfColor color);

/// <summary>
/// The function draws the specified Cell on the given SFML render window.
/// </summary>
/// <param name="cell">The cell to be drawn</param>
/// <param name="window">The SFML render window where the cell will be drawn</param>
void CellDraw(Cell* cell, sfRenderWindow* window);

/// <summary>
/// This function reveals the cell at the specified grid position in the given grid.
/// It returns true if the revealed cell is a bomb, false otherwise.
/// </summary>
/// <param name="grid">The grid where the cell is contained </param>
/// <param name="cellGridPos">The grid position of the cell to be revealed in sfVector2i format ({ x, y })</param>
int CellReveal(Grid* grid, sfVector2i cellGridPos);

/// <summary>
/// This function toggles the flagged state of the cell at the specified grid position in the given grid.
/// </summary>
/// <param name="grid">The grid where the cell is contained</param>
/// <param name="cellGridPos">The grid position of the cell to be flagged/unflagged in sfVector2i format ({ x, y })</param>
void CellFlag(Grid* grid, sfVector2i cellGridPos);

/// <summary>
/// This function destroys the specified Cell and frees its resources.
/// </summary>
/// <param name="cell"> The cell to be destroyed</param>
void CellDestroy(Cell* cell);

/// <summary>
/// This function creates and initializes a new Grid for the Minesweeper game.
/// It returns a pointer to the newly created Grid.
/// </summary>
Grid* GridCreate();

/// <summary>
/// This function plants bombs randomly in the grid, avoiding the specified cell position.
/// You want to avoid planting a bomb at the first revealed cell to ensure fair gameplay. (only for advanced part)
/// </summary>
/// <param name="grid">The grid where bombs will be planted</param>
/// <param name="bombCount">The number of bombs to be planted in the grid</param>
/// <param name="cellToAvoid">The grid position to avoid planting a bomb in sfVector2i format ({ x, y })</param>
void GridPlantBomb(Grid* grid, int bombCount, sfVector2i cellToAvoid);

/// <summary>
/// This function updates the grid based on mouse position and returns the coordinates of the currently hovered cell.
/// If no cell is hovered, it returns (-1, -1).
/// </summary>
/// <param name="grid">The grid to be updated</param>
/// <param name="window">The SFML render window to get mouse position from</param>
/// <returns>The coordinates of the currently hovered cell in sfVector2i format ({ x, y })</returns>
sfVector2i GridUpdateLoop(Grid* grid, sfRenderWindow* window);

/// <summary>
/// This function draws the entire grid on the given SFML render window.
/// </summary>
/// <param name="grid">The grid to be drawn</param>
/// <param name="window">The SFML render window where the grid will be drawn</param>
void GridDraw(Grid* grid, sfRenderWindow* window);

/// <summary>
/// This function destroys the specified Grid and frees its resources.
/// </summary>
/// <param name="grid">The grid to be destroyed</param>
void GridDestroy(Grid* grid);