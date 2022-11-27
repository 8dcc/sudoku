
#include "globals.h"

void init_grid() {
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            grid[y][x] = EMPTY_CELL;
}
