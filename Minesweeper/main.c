#include <SFML/Audio.h>
#include <SFML/Graphics.h>

#include "basics.h"
#include "Grid.h"

#define DEBUG_CLEAN false

int main(void)
{
    sfVideoMode mode = { WIDTH, HEIGHT, 32 };
    sfRenderWindow* window;
    sfEvent event;

    /* Create the main window */
    window = sfRenderWindow_create(mode, "Minesweeper", sfClose, NULL);
    if (!window)
    {
        return NULL_WINDOW;
    }

    srand((unsigned int)time(NULL));

    // Initialize the game grid
    Grid* gameGrid = GridCreate();

    // Use to register on what cell the mouse is currently pointing
    sfVector2i currentCell = { -1, -1 };

    printf("Start Game ! \n");
    bool bFirstTouch = true;

    /* Start the game loop */
    while (sfRenderWindow_isOpen(window))
    {
        if (DEBUG_CLEAN) system("cls");

        /* Process events */
        while (sfRenderWindow_pollEvent(window, &event))
        {
            /* Close window : exit */
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
            }

            // Update currentCell on mouse movements
            if (event.type == sfEvtMouseMoved)
            {
                currentCell = GridUpdateLoop(gameGrid, window);
            }

            // Make grid react if left mouse button clicked
            if (event.type == sfEvtMouseButtonPressed)
            {
                if (event.key.code == sfMouseLeft)
                {
                    if (currentCell.x >= 0 && currentCell.y >= 0)
                    {
                        //Plant bombs after first touch
                        if (bFirstTouch)
                        {
                            bFirstTouch = false;
                            GridPlantBomb(gameGrid, BOMB_COUNT, currentCell);
                        }

                        switch (CellReveal(gameGrid, currentCell))
                        {
                        case FAILURE:
                            printf("*LOUD EXPLOSION NOISE*");
                            return FAILURE;
                        case SUCCESS:
                            printf("CONGRATS YOU WIN THIS GAME !");
                            return SUCCESS;
                        default:
                            break;
                        }
                    }
                }

                if (event.key.code == sfMouseRight)
                {
                    if (currentCell.x >= 0 && currentCell.y >= 0)
                    {
                        CellFlag(gameGrid, currentCell);
                    }
                }
            }
        }


        /* Clear the screen */
        sfRenderWindow_clear(window, sfBlack);

        // Draw everything here
        GridDraw(gameGrid, window);

        /* Update the window */
        sfRenderWindow_display(window);
    }

    /* Cleanup resources */
    GridDestroy(gameGrid);
    sfRenderWindow_destroy(window);

    return SUCCESS;
}