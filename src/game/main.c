
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "globals.h"
#include "misc.h"
#include "sudoku.h"
#include "interface.h"

int grid[ROWS][COLS];
int unk_grid[ROWS][COLS];

int main(int argc, char** argv) {
    initscr();               // Init ncurses
    raw();                   // Scan input without pressing enter
    noecho();                // Don't print when typing
    keypad(stdscr, TRUE);    // Enable keypad (arrow keys)

    // Arguments for difficulty
    int difficulty = DEFAULT_DIFFICULTY;
    if (argc > 1) {
        difficulty = atoi(argv[1]);

        if (difficulty < 1 || difficulty >= ROWS * COLS)
            die("Error. Invalid argument.\n"
                "Usage:\n"
                "    %s            - Run with default difficulty.\n"
                "    %s <number>   - Where number is the number of cells that are "
                "going to be filled. [%d-%d]\n",
                argv[0], argv[0], 1, ROWS * COLS - 1);
    }

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

    // Initialize both grids to UNK
    init_grid(grid);
    init_grid(unk_grid);

    // Generate a valid sudoku
    generate_sudoku(difficulty);

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
