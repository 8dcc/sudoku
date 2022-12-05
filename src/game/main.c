
#include <stdio.h>
#include <ncurses.h>

#include "globals.h"
#include "misc.h"
#include "sudoku.h"
#include "interface.h"

int grid[ROWS][COLS];
int unk_grid[ROWS][COLS];

int main() {
    initscr();               // Init ncurses
    raw();                   // Scan input without pressing enter
    noecho();                // Don't print when typing
    keypad(stdscr, TRUE);    // Enable keypad (arrow keys)

    // Color
#ifdef USE_COLOR
    if (!has_colors())
        die("Error. Your terminal does not support color.\n"
            "You can disable it editing src/game/globals.h\n");

    start_color();

    // Make black gray
    init_color(COLOR_BLACK, 100, 100, 100);

    init_pair(NORM, COLOR_WHITE, COLOR_BLACK);
    init_pair(BOLD, COLOR_BLUE, COLOR_BLACK);
    init_pair(SOFT, COLOR_GREEN, COLOR_BLACK);
    init_pair(FCOL, COLOR_RED, COLOR_BLACK);
    init_pair(NFCOL, COLOR_BLACK, COLOR_BLACK);
#endif

    init_grid(grid);
    init_grid(unk_grid);

    // Fill the empty array with 1's where there was an UNK for showing gray chars on
    // the old positions
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            unk_grid[y][x] = (grid[y][x] == UNK);

    do {
        print_sudoku(&grid[0][0], &unk_grid[0][0]);
    } while (getch() != 'q');

    printf("Finished.\n");
    endwin();
    return 0;
}
