
/* --------------------------------------------------------- */
/* Macros */

#define REFRESH_0() \
    {               \
        move(0, 0); \
        refresh();  \
    }

#define USE_COL(col) attron(COLOR_PAIR(col))

#define RESET_COL(col) attroff(COLOR_PAIR(col))

#define BOLD_ON() attron(A_BOLD)

#define BOLD_OFF() attroff(A_BOLD)

/* --------------------------------------------------------- */
/* Defines */

/*
 * Comment this line if you don't want colors!
 * Will not print with colors if not defined, useful for redirecting the output of
 * the command.
 */
#define USE_COLOR

#define DEFAULT_DIFFICULTY   27
#define MAX_DIFFICULTY_TRIES 200    // See sudoku.c/generate_sudoku()

#define XP 5    // Position of the sudoku when drawing
#define YP 3

#define ROWS  9
#define COLS  9
#define BOXSZ 9    // Numbers in each box

#define UNK   -1    // Empty sudoku cell ('?')
#define UNK_C '?'

/* --------------------------------------------------------- */
/* Globals */

extern int grid[ROWS][COLS];
extern int unk_grid[ROWS][COLS];

/* --------------------------------------------------------- */
/* Colors */

#define NORM  1
#define BOLD  2
#define SOFT  3
#define FCOL  4
#define NFCOL 5
