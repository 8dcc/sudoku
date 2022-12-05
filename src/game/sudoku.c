
#include <stdlib.h>    // For rand()
#include <time.h>      // For time()

#include "globals.h"
#include "misc.h"
#include "sudoku.h"

static int valid_pos(int* arr, int idx, int num);

/*
do {
    filled = 0;
    while (filled is less than difficulty)
        if (random num at random pos is valid)
            sudoku[random_pos] = random_num;
            filled++;
} while (generated sudoku cant be solved)

Where difficulty is the parameter and indicates the number of cells to be filled.
*/
void generate_sudoku(int difficulty) {
    // Seed for rand()
    srand(time(NULL));

    do {
        int filled_c = 0;
        int total_c  = 0;

        // Set grid to UNK
        init_grid(grid);

        /*
         * Generate valid numbers at random positions until we filled enough. If we
         * tried too many times (we can't get a valid sudoku for the current
         * difficulty with the current tries), try an entirely new sudoku. Probably
         * not the best method but necesary since we are bruteforcing, after all.
         */
        while (filled_c < difficulty && total_c < MAX_DIFFICULTY_TRIES) {
            int rand_p = rand() % (ROWS * COLS);
            int rand_n = (rand() % 9) + 1;    // 0-8 -> 1-9

            // Get x and y from idx
            int yp, xp;
            idx2yx(rand_p, &yp, &xp);

            // If we can put the random number in the rand position and its empty
            if (grid[yp][xp] == UNK && valid_pos(&grid[0][0], rand_p, rand_n)) {
                grid[yp][xp] = rand_n;

                // Count how many we actually filled.
                filled_c++;
            }

            total_c++;
        }
    } while (0 /* TODO: check if it can't be solved and repeat */);
}

// Same as src/solver/sudoku.c
int valid_pos(int* arr, int idx, int num) {
    // Get x and y pos in the arr from idx
    int yp, xp;
    idx2yx(idx, &yp, &xp);

    // Check current row
    for (int x = 0; x < COLS; x++)
        if (arr[COLS * yp + x] == num)
            return 0;

    // Check current col
    for (int y = 0; y < ROWS; y++)
        if (arr[COLS * y + xp] == num)
            return 0;

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
        if (arr[boxes[yp / 3][xp / 3][box_i]] == num)
            return 0;

    return 1;
}

/* idx2yx: converts idx to 1d array to x and y from 2d array */
void idx2yx(int idx, int* y, int* x) {
    *y = idx / COLS;
    *x = idx - *y * COLS;
}
