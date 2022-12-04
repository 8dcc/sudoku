
/* --------------------------------------------------------- */
/* Macros */

#define REFRESH_0() \
    {               \
        move(0, 0); \
        refresh();  \
    }

/* --------------------------------------------------------- */
/* Defines */

/*
 * Comment this line if you don't want colors!
 * Will not print with colors if not defined, useful for redirecting the output of
 * the command.
 */
/* #define USE_COLOR */

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

// TODO: Pairs
#ifdef USE_COLOR
#define BOLD  TC_B_BLU
#define NORM  TC_NRM
#define SOFT  TC_GRN
#define FCOL  TC_B_RED
#define NFCOL TC_B_GRY
#else
#define BOLD  ""
#define NORM  ""
#define SOFT  ""
#define FCOL  ""
#define NFCOL ""
#endif
