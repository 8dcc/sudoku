
/* --------------------------------------------------------- */
/* Macros */

#define REFRESH_0() \
    {               \
        move(0, 0); \
        refresh();  \
    }

/* --------------------------------------------------------- */
/* Defines */

#define XP 5    // Position of the sudoku when drawing
#define YP 3
#define ROWS 9
#define COLS 9

#define EMPTY_CELL -1

/* --------------------------------------------------------- */
/* Globals */

extern int grid[ROWS][COLS];
