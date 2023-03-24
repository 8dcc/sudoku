
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>    // For tolower()
#include <ncurses.h>

#include "globals.h"
#include "misc.h"
#include "sudoku.h"
#include "interface.h"

// Initialize externs from globals.h
int grid[ROWS][COLS];
int unk_grid[ROWS][COLS];
int solved[ROWS][COLS];
int altered_sudoku = 0;

int main(int argc, char** argv) {
    initscr();               // Init ncurses
    raw();                   // Scan input without pressing enter
    noecho();                // Don't print when typing
    keypad(stdscr, TRUE);    // Enable keypad (arrow keys)

    // Arguments for difficulty
    int difficulty = DEFAULT_DIFFICULTY;
    if (argc > 1) {
        difficulty = atoi(argv[1]);

        if (strncmp(argv[1], "-h", 2) == 0 || strncmp(argv[1], "--help", 6) == 0)
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

    // Initialize grids to UNK
    init_grid(grid);
    init_grid(unk_grid);
    init_grid(solved);

    // If difficulty is high, generation might take a while
    if (difficulty > MIN_LOADING_DIFICULTY) {
        mvprintw(1, 2, "Generating sudoku with difficulty %d...", difficulty);
        refresh();
    }

    // Generate a valid sudoku
    generate_sudoku(difficulty);

    // Clear line after generating
    if (difficulty > MIN_LOADING_DIFICULTY) {
        CLEAR_LINE(1);
        refresh();
    }

    /* Fill the empty array with 1's where there was an UNK for showing gray chars on
     * the old positions:
     *   grid     = { 5, UNK, 7, 8, UNK, ... }
     *   unk_grid = { 0,   1, 0, 0,   1, ... }
     */
    get_unk(&grid[0][0], &unk_grid[0][0]);

    int cursor_y = 0, cursor_x = 0;
    init_cursor(&cursor_y, &cursor_x, &unk_grid[0][0]);

    // Show keybinds before the main loop
    SHOW_HELP_TITLE("Keybinds:");
    SHOW_HELP(0, "Arrows", "Move through the sudoku");
    SHOW_HELP(1, "hjkl", "Move through the sudoku");
    SHOW_HELP(2, "1-9", "Change state of unknown cell");
    SHOW_HELP(3, "s", "Solve the sudoku in the current state");
    SHOW_HELP(4, "g", "Generate a new sudoku");
    SHOW_HELP(5, "q", "Quit");

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
                if (difficulty > MIN_LOADING_DIFICULTY) {
                    OUTPUT_MSG("Generating sudoku with difficulty %d...",
                               difficulty);
                    REFRESH_0();
                }

                generate_sudoku(difficulty);

                // Update unknown positions
                get_unk(&grid[0][0], &unk_grid[0][0]);

                // Move cursor to first unknown cell
                init_cursor(&cursor_y, &cursor_x, &unk_grid[0][0]);

                // Will refresh in next iteration of the main loop
                if (difficulty > MIN_LOADING_DIFICULTY)
                    OUTPUT_MSG("Finished generating sudoku");

                break;
            case 's':
                // If we altered the sudoku since we generated it (solved array is
                // not valid), solve it again.
                if (altered_sudoku) {
                    // Try to solve, if we can't, show error and stop
                    if (!solve(&grid[0][0], &solved[0][0])) {
                        OUTPUT_MSG("Current sudoku can't be solved!");
                        break;
                    }
                }

                // Save current grid as the original one for unk values (gray),
                // in case user made changes.
                get_unk(&grid[0][0], &unk_grid[0][0]);

                // We don't need to solve, now we just gotta show the solved array
                // See generate_sudoku()

                // If it can be solved, replace
                copy_grid(&solved[0][0], &grid[0][0]);

                // Move cursor to first unknown cell
                init_cursor(&cursor_y, &cursor_x, &unk_grid[0][0]);

                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                write_cell(&cursor_y, &cursor_x, &grid[0][0], &unk_grid[0][0], c);
                break;
            case 'k':
            case KEY_UARROW:
                move_cursor(&cursor_y, &cursor_x, &unk_grid[0][0], UP);
                break;
            case 'j':
            case KEY_DARROW:
                move_cursor(&cursor_y, &cursor_x, &unk_grid[0][0], DOWN);
                break;
            case 'h':
            case KEY_LARROW:
                move_cursor(&cursor_y, &cursor_x, &unk_grid[0][0], LEFT);
                break;
            case 'l':
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
