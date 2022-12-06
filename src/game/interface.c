
#include <ncurses.h>

#include "globals.h"

// See src/solver/sudoku.c
void print_sudoku(int* arr, int* unk_arr) {
    move(YP, XP);

    // Each iteration of y prints 2 lines, the separator before the row, and th row
    for (int y = 0; y < ROWS; y++) {
        if (y % 3 == 0) {
            BOLD_ON();
            USE_COL(BOLD);
        }

        for (int x = 0; x < COLS; x++) {
            if (y % 3 != 0 && x % 3 == 0) {
                BOLD_ON();
                USE_COL(BOLD);
                printw("+");
                BOLD_OFF();
                USE_COL(SOFT);
                printw("---");
            } else {
                printw("+---");
            }
        }
        BOLD_ON();
        USE_COL(BOLD);
        printw("+");
        BOLD_OFF();
        USE_COL(NORM);
        move(YP + y * 2 + 1, XP);

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
#ifdef USE_COLOR
            int NUM_COL = (unk_arr[idx]) ? NFCOL : FCOL;
#endif

            // For showing the main 9 squares (cols)
            if (x % 3 == 0) {
                BOLD_ON();
                USE_COL(BOLD);
            } else {
                BOLD_OFF();
                USE_COL(SOFT);
            }

            printw("|");
            BOLD_ON();    // Number always bold
            USE_COL(NUM_COL);
            printw(" %c ", c);
            BOLD_OFF();
            USE_COL(NORM);
        }
        BOLD_ON();
        USE_COL(BOLD);
        printw("|");
        BOLD_OFF();
        USE_COL(NORM);
        move(YP + y * 2 + 2, XP);
    }

    BOLD_ON();
    USE_COL(BOLD);
    // Border after last row (bold too)
    for (int x = 0; x < COLS; x++)
        printw("+---");
    printw("+");
    BOLD_OFF();
    USE_COL(NORM);

    REFRESH_0();
}
