
#include <stdio.h>
#include <ctype.h>    // isdigit()
#include "defines.h"

void init_arr(int* arr) {
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            arr[COLS * y + x] = UNK;
}

static inline int is_valid(int c) {
    // Digit or '?'
    return isdigit(c) || c == UNK_C;
}

static inline int c2i(int c) {
    // Convert char to int or UNK_C ('?') to UNK int (-1)
    return (c == UNK_C) ? UNK : c - '0';
}

int read_arr(FILE* fd, int* arr) {
    if (!fd || !arr)
        return 1;

    int c = 0;
    int x = 0, y = 0;

    while ((c = getc(fd)) != EOF) {
        // We need the >= instead of just > because arr[COLS] would be out of bounds.
        // Same with the rows.
        if (x >= COLS && y < ROWS) {
            x = 0;
            y++;
        }

        // We use the < to check if we are in bounds, keep in mind COLS is the number
        // of items and not the last valid idx.
        if (is_valid(c) && x < COLS) {
            arr[COLS * y + x] = c2i(c);
            x++;
        }
    }

    return 0;
}

void print_arr(int* arr) {
    char c;
    char* NUM_COL;

    for (int y = 0; y < ROWS; y++) {
        // For showing the main 9 squares (rows)
        if (y % 3 == 0)
            printf("%s", BOLD);

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
            printf("%s", NORM);

        // Row itself
        for (int x = 0; x < COLS; x++) {
            c       = arr[ROWS * y + x];
            NUM_COL = FCOL;    // Char color when found

            // If array pos is UNK (-1), change color and char
            if (c == UNK) {
                c       = UNK_C;    // '?'
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
    printf("%s", BOLD);
    for (int x = 0; x < COLS; x++)
        printf("+---");
    printf("+\n");
    printf("%s", NORM);
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

