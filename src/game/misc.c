
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

// Sorry intel assembly syntax
void copy_grid(int* src, int* dest) {
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            dest[COLS * y + x] = src[COLS * y + x];
}

// Puts 1 in unk_arr if that cell in arr is UNK
void get_unk(int* arr, int* unk_arr) {
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            unk_arr[COLS * y + x] = (arr[COLS * y + x] == UNK);
}

void die(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);

    endwin();
    vfprintf(stderr, fmt, va);

    va_end(va);
    exit(1);
}
