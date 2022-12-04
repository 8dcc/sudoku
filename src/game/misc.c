
#include <stdio.h>      // For vfprintf
#include <stdlib.h>     // For exit
#include <stdarg.h>     // For va_list
#include <ncurses.h>    // For endwin()

#include "globals.h"

void init_grid(int arr[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            arr[y][x] = UNK;
}

void die(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);

    endwin();
    vfprintf(stderr, fmt, va);

    va_end(va);
    exit(1);
}
