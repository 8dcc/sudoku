
#include <ncurses.h>

#include "globals.h"
#include "sudoku.h"    // For valid_pos

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

#ifdef USE_COLOR
            // If we know the current item should be hidden (is 1 in the array),
            // change colors.
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

// Arr cell to position on screen
static inline void arr2screen(int* y, int* x) {
    *y = YP + (*y * 2) + 1;
    *x = XP + (*x * 4) + 2;
}

// Screen position to arr cell
static inline void screen2arr(int* y, int* x) {
    *y = (*y - YP - 1) / 2;
    *x = (*x - XP - 2) / 4;
}

// Returns the closest unknown cell on the current Y. If both deltas are the same, it
// will go to the left.
static inline int closest_unk(int oy, int ox, int* unk_grid) {
    int l_delta = COLS;
    int r_delta = COLS;
    int x;

    // Get delta from closest to the left
    for (x = ox; x >= 0; x--) {
        if (unk_grid[COLS * oy + x]) {
            l_delta = ox - x;
            break;
        }
    }
    // Get delta from closest to the right
    for (x = ox + 1; x < COLS; x++) {
        if (unk_grid[COLS * oy + x]) {
            r_delta = x - ox;
            break;
        }
    }

    // If the closest unknown is to the left, subtract the left delta, else,
    // add the right delta.
    x = (l_delta <= r_delta) ? ox - l_delta : ox + r_delta;

    // Return x if in range, original x otherwise
    return (x >= 0 && x < COLS) ? x : ox;
}

// Set cx and cy to the screen position of the first unknown cell
void init_cursor(int* cy, int* cx, int* unk_grid) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (unk_grid[COLS * y + x]) {
                arr2screen(&y, &x);
                *cy = y;
                *cx = x;
                return;
            }
        }
    }

    *cy = 0;
    *cx = 0;
}

// Move cursor to the next unknown cell depending on dir
void move_cursor(int* cy, int* cx, int* unk_grid, int dir) {
    int arr_y = *cy, arr_x = *cx;
    screen2arr(&arr_y, &arr_x);

    switch (dir) {
        case UP:
            // Decrease y (move up)
            if (arr_y > 0)
                arr_y--;

            // Get closest unknown in the new Y
            arr_x = closest_unk(arr_y, arr_x, unk_grid);

            // Convert to screen coordinates and write to cursor
            arr2screen(&arr_y, &arr_x);
            *cy = arr_y;
            *cx = arr_x;

            break;
        case DOWN:
            // Increase y (move down)
            if (arr_y < ROWS - 1)
                arr_y++;

            // Get closest unknown in the new Y
            arr_x = closest_unk(arr_y, arr_x, unk_grid);

            // Convert to screen coordinates and write to cursor
            arr2screen(&arr_y, &arr_x);
            *cy = arr_y;
            *cx = arr_x;

            break;
        case LEFT:
            // Go from the current x to 0, until we encounter an unk cell
            for (int x = arr_x - 1; x >= 0; x--) {
                if (unk_grid[COLS * arr_y + x]) {
                    arr2screen(&arr_y, &x);
                    *cy = arr_y;
                    *cx = x;
                    break;
                }
            }

            break;
        case RIGHT:
            // Go from the current x to the end of the row, until we encounter an unk
            // cell.
            for (int x = arr_x + 1; x < COLS; x++) {
                if (unk_grid[COLS * arr_y + x]) {
                    arr2screen(&arr_y, &x);
                    *cy = arr_y;
                    *cx = x;
                    break;
                }
            }

            break;
        default:
            OUTPUT_MSG("move_cursor: unknown dir %d", dir);
            break;
    }
}

void write_cell(int* cy, int* cx, int* grid, int* unk_grid, int c) {
    int y = *cy, x = *cx;
    screen2arr(&y, &x);

    // Char out of bounds
    if (c < '1' || c > '9') {
        OUTPUT_MSG("Could not write char %c (%d) to cell.", c, c);
        return;
    }

    // If the cell on the original grid is not unknown
    if (!unk_grid[COLS * y + x]) {
        OUTPUT_MSG("Tried to write to a known cell at (y%d, x%d)", y, x);
        return;
    }

    // Digit to int
    c -= '0';

    // If it's there, it would be invalid so don't do anything else
    if (grid[COLS * y + x] == c)
        return;

    // If the current number is not valid
    if (!valid_pos(grid, COLS * y + x, c)) {
        OUTPUT_MSG("Invalid position for '%d' at (y%d, x%d)", c, y, x);
        return;
    }

    grid[COLS * y + x] = c;
    altered_sudoku     = 1;
}

