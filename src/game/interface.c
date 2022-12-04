
#include <ncurses.h>

#include "globals.h"

// See src/solver/sudoku.c
void print_sudoku(int* arr, int* unk_arr) {
    move(YP, XP);

    char* NUM_COL;

    // Each iteration of y prints 2 lines, the separator before the row, and th row
    for (int y = 0; y < ROWS; y++) {
        if (y % 3 == 0)
            printw("%s", BOLD);

        for (int x = 0; x < COLS; x++) {
            if (y % 3 != 0 && x % 3 == 0)
                printw(BOLD "+" SOFT "---");
            else
                printw("+---");
        }
        printw(BOLD "+" NORM);
        move(YP + y * 2 + 1, XP);

        // Should not be necesary
        if (y % 3 == 0)
            printw("%s", NORM);

        // Row itself
        for (int x = 0; x < COLS; x++) {
            int idx = COLS * y + x;

            int c = arr[idx];

            // If array pos is UNK (-1), change char
            if (c == UNK)
                c = UNK_C;    // '?'
            else
                c += '0';

            // If we know the current item should be hidden (is 1 in the array),
            // change colors.
            NUM_COL = (unk_arr[idx]) ? NFCOL : FCOL;

            // For showing the main 9 squares (cols)
            if (x % 3 == 0)
                printw(BOLD "|%s %c " NORM, NUM_COL, c);
            else
                printw(SOFT "|%s %c " NORM, NUM_COL, c);
        }
        printw(BOLD "|" NORM);
        move(YP + y * 2 + 2, XP);
    }

    // Border after last row (bold too)
    printw("%s", BOLD);
    for (int x = 0; x < COLS; x++)
        printw("+---");
    printw("+" NORM);

    REFRESH_0();
}
