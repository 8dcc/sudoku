#include <ncurses.h>

#include "globals.h"

void print_sudoku() {
    mvprintw(YP, XP, "I am a test!");
    REFRESH_0();
}
