
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
         * If there is no filename as argument and we are in linux, check if we are
         * using a pipe or similar. If not, exit (don't read from user).
         *
         * We need to check if we are in windows because the function that allows us
         * to check if we are using pipes is from unistd, so we would just read from
         * stdin on windows without checking for pipes.
         */
        if (isatty(0)) {
            die("Usage: \n"
                "   %s file.txt\n"
                "   %s < file.txt\n",
                argv[0], argv[0]);
        }
#endif
    }

    int arr[ROWS][COLS];
    int* arr_p = &arr[0][0];
    init_arr(arr_p);

    int old_empty[ROWS][COLS];
    int* old_empty_p = &old_empty[0][0];
    init_arr(old_empty_p);

    if (read_arr(fd, arr_p) != 0)
        die("Error reading the file.\nfd:  %p\narr: %p\n", fd, arr_p);

    // Fill the empty array with 1's where there was an UNK for showing gray chars on
    // the old positions after solving.
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            old_empty[y][x] = (arr[y][x] == UNK);

    print_unk_arr(arr_p);
    solve(arr_p);

    printf(NFCOL "\n| | | | | | | | | | | | | | | | | | |\n"
                 "v v v v v v v v v v v v v v v v v v v\n\n" NORM);

    print_arr(arr_p, old_empty_p);

    fclose(fd);
    return 0;
}

