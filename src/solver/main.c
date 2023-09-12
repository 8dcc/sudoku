
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef __unix__
#include <unistd.h>    // isatty()
#endif

#include "defines.h"
#include "misc.h"
#include "sudoku.h"

int main(int argc, char** argv) {
    FILE* fd = stdin;

    if (argc > 1 && strcmp(argv[1], "-") != 0) {
        fd = fopen(argv[1], "r");
        if (!fd)
            die("Error. Can't read file \"%s\"\n", argv[1]);
#ifdef __unix__
    } else {
        /*
         * If there is no filename as argument and we are in linux, check if we
         * are using a pipe or similar. If not, exit (don't read from user).
         *
         * We need to check if we are in windows because the function that
         * allows us to check if we are using pipes is from unistd, so we would
         * just read from stdin on windows without checking for pipes.
         */
        if (isatty(0)) {
            die("Usage: \n"
                "   %s file.txt\n"
                "   %s < file.txt\n",
                argv[0], argv[0]);
        }
#endif
    }

    int arr[ROWS * COLS] = { 0 };
    init_arr(arr);

    if (read_arr(fd, arr) != 0)
        die("Error reading the file.\n"
            "fd:  %p\n"
            "arr: %p\n",
            fd, arr);

    bool empty[ROWS * COLS] = { false };

    /* Store in the empty array the cells that had UNK on them. Used to change
     * colors of chars on the old unknown positions after solving. */
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            empty[y * COLS + x] = (arr[y * COLS + x] == UNK);

    print_arr(arr, empty);
    printf(TC_NRM "\n| | | | | | | | | | | | | | | | | | |\n"
                  "v v v v v v v v v v v v v v v v v v v\n\n" NORM);

    solve(arr);
    print_arr(arr, empty);

    fclose(fd);
    return 0;
}
