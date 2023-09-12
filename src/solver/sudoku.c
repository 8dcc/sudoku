
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h> /* isdigit() */

#include "sudoku.h"
#include "defines.h"
#include "misc.h"

/*----------------------------------------------------------------------------*/

static inline int is_valid(int c) {
    /* Digit or '?' */
    return isdigit(c) || c == UNK_C;
}

static inline int c2i(int c) {
    /* Not a char, invalid */
    if (!is_valid(c))
        die("c2i: tried to convert complex integer (%d) to char.\n", c);

    /* Convert char to int or UNK_C ('?') to UNK int (-1) */
    return (c == UNK_C) ? UNK : c - '0';
}

/*----------------------------------------------------------------------------*/

void init_arr(int* arr) {
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            arr[y * COLS + x] = 0;
}

int read_arr(FILE* fd, int* arr) {
    if (!fd || !arr)
        return 1;

    int c = 0;
    int x = 0, y = 0;

    while ((c = getc(fd)) != EOF && y < ROWS) {
        if (x >= COLS) {
            x = 0;
            y++;
        }

        if (is_valid(c) && x < COLS) {
            arr[y * COLS + x] = c2i(c);
            x++;
        }
    }

    return 0;
}

/* The was_unk array is used to know if each cell was unknown before solving. If
 * it was, we print it with a different color. It's filled in main() */
void print_arr(int* arr, bool* was_unk) {
    char* NUM_COL;

    for (int y = 0; y < ROWS; y++) {
        /* For showing the main 9 squares (rows) */
        if (y % 3 == 0)
            printf("%s", BOLD);

        /* Border before each row */
        for (int x = 0; x < COLS; x++) {
            if (y % 3 != 0 && x % 3 == 0)
                printf(BOLD "+" SOFT "---");
            else
                printf("+---");
        }

        /* Last '+' is always bold */
        printf(BOLD "+" NORM "\n");

        /* Reset colors if we just drew a bold one */
        if (y % 3 == 0)
            printf("%s", NORM);

        /* Row itself */
        for (int x = 0; x < COLS; x++) {
            int idx = COLS * y + x;
            int c   = arr[idx];

            /* Convert to char */
            if (c == UNK)
                c = UNK_C; /* '?' */
            else
                c += '0'; /* 5 -> '5' */

            /* Change color if it was hidden before solving */
            NUM_COL = was_unk[idx] ? NFCOL : FCOL;

            /* For showing the main 9 squares (cols) */
            if (x % 3 == 0)
                printf(BOLD "|%s %c " NORM, NUM_COL, c);
            else
                printf(SOFT "|%s %c " NORM, NUM_COL, c);
        }
        printf(BOLD "|" NORM "\n");
    }

    /* Border after last row */
    printf("%s", BOLD);
    for (int x = 0; x < COLS; x++)
        printf("+---");
    printf("+" NORM "\n");
}

static int valid_pos(int* arr, int idx, int num) {
    /* Get x and y pos in the arr from idx */
    const int yp = idx / COLS;
    const int xp = idx - yp * COLS;

    /* Check current row */
    for (int x = 0; x < COLS; x++)
        if (arr[yp * COLS + x] == num)
            return 0;

    /* Check current col */
    for (int y = 0; y < ROWS; y++)
        if (arr[y * COLS + xp] == num)
            return 0;

    /* Idx of cells corresponding to each box by Y and X of the box */
    static const int boxes[COLS][ROWS][BOXSZ] = {
        {
          /* Top row of boxes */
          { 0, 1, 2, 9, 10, 11, 18, 19, 20 },  /* y0, x0 */
          { 3, 4, 5, 12, 13, 14, 21, 22, 23 }, /* y0, x1 */
          { 6, 7, 8, 15, 16, 17, 24, 25, 26 }  /* y0, x2 */
        },
        {
          /* Middle row of boxes */
          { 27, 28, 29, 36, 37, 38, 45, 46, 47 }, /* y1, x0 */
          { 30, 31, 32, 39, 40, 41, 48, 49, 50 }, /* y1, x1 */
          { 33, 34, 35, 42, 43, 44, 51, 52, 53 }  /* y1, x2 */
        },
        {
          /* Bottom row of boxes */
          { 54, 55, 56, 63, 64, 65, 72, 73, 74 }, /* y2, x0 */
          { 57, 58, 59, 66, 67, 68, 75, 76, 77 }, /* y2, x1 */
          { 60, 61, 62, 69, 70, 71, 78, 79, 80 }  /* y2, x2 */
        },
    };

    /* Get current box idx from YP and XP, then get array of indexes for that */
    const int box_y            = yp / 3;
    const int box_x            = xp / 3;
    const int* cur_box_indexes = &boxes[box_y][box_x][0];

    /* Check if number is in current box by checking each valid index */
    for (int box_i = 0; box_i < BOXSZ; box_i++) {
        const int idx_to_check = cur_box_indexes[box_i];
        if (arr[idx_to_check] == num)
            return 0;
    }

    return 1;
}

/*
 * arr[ROWS][COLS] = {
 *     { 5, 3, ?, ?, 7, ?, ?, ?, ? },
 *     { 6, ?, ?, 1, 9, 5, ?, ?, ? },
 *     ...
 * };
 *
 * Where ? is stored as UNK (-1).
 * We will locally store the raw indexes (2d -> 1d) of those UNK values in
 * another array called unk_indexes.
 */
void solve(int* arr) {
    /* Array that will hold a list of the unknown indexes of arr. */
    int unk_indexes[ROWS * COLS] = { 0 };
    int unk_indexes_sz           = 0;

    /* Store unknown cells of arr in unk_indexes. Also store array size. */
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            if (arr[y * COLS + x] == UNK)
                unk_indexes[unk_indexes_sz++] = y * COLS + x;

    /* Iterate unkown cells in unk_indexes */
    for (int i = 0; i < unk_indexes_sz; i++) {
        /* Get the current index we want to check from unk_indexes array */
        int cur_idx = unk_indexes[i];

        /* Will store the first valid number we can use on the current cell */
        int valid = 0;

        /* Try 1 to 9 for current unknown cell. If the current cell we are
         * checking has a value, use that one. Start from 1 otherwise. */
        int starting_value = (arr[cur_idx] == UNK) ? 1 : arr[cur_idx];
        for (int num = starting_value; num <= 9; num++) {
            if (valid_pos(arr, cur_idx, num)) {
                valid = num;
                break;
            }
        }

        /* If we found a valid number, set it and check the next unknown */
        if (valid != 0) {
            arr[cur_idx] = valid;
            continue;
        }

        /* If this is the first unknown index we are checking, and it didn't
         * have a solution, mark as unsolvable. */
        if (i <= 0)
            die("solve: no valid number for the first unknown cell\n");

        /* Not the first unknown index, reset the cell to UNK and go back to the
         * previous unknown value from the unk_indexes array. We subtract 2
         * because the for loop will ad 1 */
        arr[cur_idx] = UNK;
        i -= 2;
    }
}
