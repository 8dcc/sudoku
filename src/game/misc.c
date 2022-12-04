
#include "globals.h"

void init_grid(int arr[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            arr[y][x] = UNK;
}
