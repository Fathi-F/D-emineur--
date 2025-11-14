#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/// <summary>
/// Use this enum to define some parameters of the window
/// </summary>
enum WindowParam {
    WIDTH = 800,
    HEIGHT = 800,
};

/// <summary>
/// Use this enum to define some parameters of the Minesweeper grid
/// </summary>
enum MinesweeperParam {
    GRID_SIZE = 10,
    GRID_OFFSET = 50,
    CELL_SIZE = 66,
    CELL_OFFSET = 4,
    BOMB_COUNT = 15,
};

/// <summary>
/// Use this enum to define some exit codes for the program (just for debug purpose)
/// </summary>
enum ExitCode {
    FAILURE = -1,
    SUCCESS = 1,
    NULL_TEXT = 60,
    NULL_WINDOW = 61,
    NULL_SPRITE = 62,
    NULL_TEXTURE = 63,
    NULL_FONT = 64,
};