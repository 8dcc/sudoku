
#include <stdio.h>
#include "defines.h"

void init_arr(int* arr) {
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            arr[ROWS * y + x] = UNK;

    // DELME
    arr[ROWS * 5 + 3] = 4;
    arr[ROWS * 2 + 1] = 6;
    arr[ROWS * 0 + 0] = 1;
}

void print_arr(int* arr) {
    char c;
    char* NUM_COL;

    for (int y = 0; y < ROWS; y++) {
        // For showing the main 9 squares (rows)
        if (y % 3 == 0)
            printf(BOLD);

        // Border before each row
        for (int x = 0; x < COLS; x++) {
            // For showing the main 9 squares ('+' chars of row separators)
            // Only leave "---" part white if we are not in a full bold row.
            if (y % 3 != 0 && x % 3 == 0)
                printf(BOLD "+" SOFT "---");
            else
                printf("+---");
        }
        // Last '+' is always bold
        printf(BOLD "+\n");

        // Reset colors if we just drew a bold one
        if (y % 3 == 0)
            printf(NORM);

        // Row itself
        for (int x = 0; x < COLS; x++) {
            c       = arr[ROWS * y + x];
            NUM_COL = FCOL;    // Char color when found

            // If array pos is UNK (-1), change color and char
            if (c == UNK) {
                c       = '?';
                NUM_COL = NFCOL;    // If we are printing '?', change color
            } else {
                c += '0';
            }

            // For showing the main 9 squares (cols)
            if (x % 3 == 0)
                printf(BOLD "|%s %c " NORM, NUM_COL, c);
            else
                printf(SOFT "|%s %c " NORM, NUM_COL, c);
        }
        printf(BOLD "|" NORM "\n");
    }

    // Border after last row (bold too)
    printf(BOLD);
    for (int x = 0; x < COLS; x++)
        printf("+---");
    printf("+\n");
    printf(NORM);
}

/*
 * arr[ROWS][COLS] = {
 *     { 5, 3, ?, ?, 7, ?, ?, ?, ? },
 *     { 6, ?, ?, 1, 9, 5, ?, ?, ? },
 *     ...
 * };
 *
 * Where ? is UNK (-1).
 */
void solve(int* arr) {}

