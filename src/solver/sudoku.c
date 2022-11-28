
#include <stdio.h>
#include <ctype.h>    // isdigit()
#include "defines.h"
#include "misc.h"

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
    // Not a char, invalid
    if (!is_valid(c))
        die("c2i: tried to convert complex integer (%d) to char.\n", c);

    // Convert char to int or UNK_C ('?') to UNK int (-1)
    return (c == UNK_C) ? UNK : c - '0';
}

int read_arr(FILE* fd, int* arr) {
    if (!fd || !arr)
        return 1;

    int c = 0;
    int x = 0, y = 0;

    while ((c = getc(fd)) != EOF && y < ROWS) {
        // We need the >= instead of just > because arr[COLS] would be out of bounds.
        // Same with the rows.
        if (x >= COLS) {
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
        printf(BOLD "+" NORM "\n");

        // Reset colors if we just drew a bold one
        if (y % 3 == 0)
            printf("%s", NORM);

        // Row itself
        for (int x = 0; x < COLS; x++) {
            c       = arr[COLS * y + x];
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
    printf("+" NORM "\n");
}

static int valid_pos(int* arr, int idx, int num) {
    // Get x and y pos in the arr from idx
    const int yp = idx / COLS;
    const int xp = idx - yp * COLS;

    // Check current row
    for (int x = 0; x < COLS; x++)
        if (arr[COLS * yp + x] == num) {
            // DELME
            /* printf("(x%dy%d) Invalid row pos for cell number %d\n", xp, yp, num);
             */
            return 0;
        }

    // Check current col
    for (int y = 0; y < ROWS; y++)
        if (arr[COLS * y + xp] == num) {
            // DELME
            /* printf("(x%dy%d) Invalid col pos for cell number %d\n", xp, yp, num);
             */
            return 0;
        }

    // Idx of cells corresponding to each box by y and x of the box
    static const int boxes[COLS][ROWS][BOXSZ] = {
        // First row of boxes (y1)
        {
          { 0, 1, 2, 9, 10, 11, 18, 19, 20 },     // y0x0
          { 3, 4, 5, 12, 13, 14, 21, 22, 23 },    // y0x1
          { 6, 7, 8, 15, 16, 17, 24, 25, 26 }     // y0x2
        },
        // Middle row of boxes (y2)
        {
          { 27, 28, 29, 36, 37, 38, 45, 46, 47 },    // y1x0
          { 30, 31, 32, 39, 40, 41, 48, 49, 50 },    // y1x1
          { 33, 34, 35, 42, 43, 44, 51, 52, 53 }     // y1x2
        },
        // Bottom row of boxes
        {
          { 54, 55, 56, 63, 64, 65, 72, 73, 74 },    // y2x0
          { 57, 58, 59, 66, 67, 68, 75, 76, 77 },    // y2x1
          { 60, 61, 62, 69, 70, 71, 78, 79, 80 }     // y2x2
        }
    };

    // Check current box
    for (int box_i = 0; box_i < BOXSZ; box_i++)
        /*
         * Divide x and y by 3 to get the box position (top-left, bottom-middle,
         * ...), use them to specify which box we want to check. We are iterating the
         * items inside that box to check if the number we want to check (num) is any
         * of the indexes that form that box.
         */
        if (arr[boxes[yp / 3][xp / 3][box_i]] == num) {
            // DELME
            /* printf("(x%dy%d) Invalid box pos for cell number %d\n", xp, yp, num);
             */
            return 0;
        }

    // DELME
    /* printf("(x%dy%d) Valid pos for cell number %d\n", xp, yp, num); */

    return 1;
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
void solve(int* arr) {
    /*
     * Array that will hold a list of the unknown indexes of arr. Assuming 3 and 5
     * are known:
     *   ucells = { 1, 2, 4, 6, 7, ... };
     */
    int ucells[ROWS * COLS] = { 0 };
    int ucells_last         = 0;

    // Check each cell of arr. If the current cell is unknown, save idx in the ucells
    // array. ucells_last will be the last valid idx + 1
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            if (arr[COLS * y + x] == UNK)
                ucells[ucells_last++] = COLS * y + x;

    // Used to check if we found a valid number when checking the unknown cells. If
    // we check all numbers and none is valid, we will go back to the previous
    // unknown cell and increase the value by 1.
    int found_valid = 0;

    // Each unknown cell idx
    for (int i = 0; i < ucells_last; i++) {
        found_valid = 0;

        // Try numbers from 1 to 9. If the current unknown cell has been written
        // before (has a val), start from there.
        int first_check = (arr[ucells[i]] == UNK) ? 1 : arr[ucells[i]];
        for (int n = first_check; n <= 9; n++) {
            if (valid_pos(arr, ucells[i], n)) {
                found_valid = n;
                break;
            }
        }

        // A cell can't be 0
        if (found_valid > 0) {
            arr[ucells[i]] = found_valid;
        } else {
            // If we are on the first cell and we can't find a valid num, sudoku is
            // unsolvable.
            if (i < 1)
                die("solve: can't find a valid number for the first unknown cell\n",
                    ucells[0]);

            // Reset the cell we could not find to -1
            arr[ucells[i]] = UNK;

            // If no number is valid in the current cell, go back to the previous one
            // and start from there. We do -2 because it's gonna do i++ after
            // continue.
            i -= 2;
        }
    }
}

