
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>    // For tolower()
#include <ncurses.h>

#include "globals.h"
#include "misc.h"
#include "sudoku.h"
#include "interface.h"

int grid[ROWS][COLS];
int unk_grid[ROWS][COLS];
int solved[ROWS][COLS];

int main(int argc, char** argv) {
    initscr();               // Init ncurses
    raw();                   // Scan input without pressing enter
    noecho();                // Don't print when typing
    keypad(stdscr, TRUE);    // Enable keypad (arrow keys)

    // Arguments for difficulty
    int difficulty = DEFAULT_DIFFICULTY;
    if (argc > 1) {
        difficulty = atoi(argv[1]);

        if (strcmp(argv[1], "--help") == 0)
            die("Usage:\n"
                "    %s            - Run with default difficulty.\n"
                "    %s <number>   - Where number is the number of cells that are "
                "going to be filled. [%d-%d]\n",
                argv[0], argv[0], 1, ROWS * COLS - 1);
        else if (difficulty < 1 || difficulty >= ROWS * COLS)
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

    SHOW_HELP_TITLE("Keybinds:");
    SHOW_HELP(0, "Arrows", "Move through the sudoku.");
    SHOW_HELP(1, "0-9", "Change state of unknown cell (WIP).");
    SHOW_HELP(2, "s", "Solve the sudoku in the current state.");
    SHOW_HELP(3, "g", "Generate a new sudoku.");
    SHOW_HELP(4, "q", "Quit.");

    // Initialize grids to UNK
    init_grid(grid);
    init_grid(unk_grid);
    init_grid(solved);

    // Generate a valid sudoku
    generate_sudoku(difficulty);

    /* Fill the empty array with 1's where there was an UNK for showing gray chars on
     * the old positions:
     *   grid     = { 5, UNK, 7, 8, UNK, ... }
     *   unk_grid = { 0,   1, 0, 0,   1, ... }
     */
    get_unk(&grid[0][0], &unk_grid[0][0]);

    int cursor_y = 0, cursor_x = 0;
    init_cursor(&cursor_y, &cursor_x, &unk_grid[0][0]);

    int c = 0;    // Char the user is pressing
    do {
        print_sudoku(&grid[0][0], &unk_grid[0][0]);
        move(cursor_y, cursor_x);

        // Get user input
        c = tolower(getch());

        // Clear output line and parse it
        CLEAR_LINE(MSG_POS + 1);
        switch (c) {
            case 'g':
                // In case it takes some time (big numbers, ~50-80)
                OUTPUT_MSG("Generating sudoku with difficulty %d...", difficulty);
                REFRESH_0();

                generate_sudoku(difficulty);

                // Update unknown positions
                get_unk(&grid[0][0], &unk_grid[0][0]);

                // Move cursor to first unknown cell
                init_cursor(&cursor_y, &cursor_x, &unk_grid[0][0]);

                // Will refresh in next iteration of the main loop
                OUTPUT_MSG("Finished generating sudoku");

                break;
            case 's':
                // We don't need to solve because we already solved when calling
                // generate_sudoku(), now we just gotta show the solved array.

                // Save current grid as the original one for unk values (gray),
                // in case user made changes.
                get_unk(&grid[0][0], &unk_grid[0][0]);

                // If it can be solved, replace
                copy_grid(&solved[0][0], &grid[0][0]);

                // Move cursor to first unknown cell
                init_cursor(&cursor_y, &cursor_x, &unk_grid[0][0]);

                break;
            case KEY_UARROW:
                move_cursor(&cursor_y, &cursor_x, &unk_grid[0][0], UP);
                break;
            case KEY_DARROW:
                move_cursor(&cursor_y, &cursor_x, &unk_grid[0][0], DOWN);
                break;
            case KEY_LARROW:
                move_cursor(&cursor_y, &cursor_x, &unk_grid[0][0], LEFT);
                break;
            case KEY_RARROW:
                move_cursor(&cursor_y, &cursor_x, &unk_grid[0][0], RIGHT);
                break;
            case KEY_CTRLC:
                c = 'q';
                break;
            case 'q':
            default:
                break;
        }
    } while (c != 'q');

    endwin();
    return 0;
}
