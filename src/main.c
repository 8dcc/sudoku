
#include <stdio.h>
#include <ncurses.h>

#include "globals.h"
#include "misc.h"
#include "interface.h"

int grid[ROWS][COLS];

int main() {
    initscr();               // Init ncurses
    raw();                   // Scan input without pressing enter
    noecho();                // Don't print when typing
    keypad(stdscr, TRUE);    // Enable keypad (arrow keys)

    init_grid();

    do {
        print_sudoku();
    } while (getch() != 'q');

    printf("Finished.\n");
    endwin();
    return 0;
}
