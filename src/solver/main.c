
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

    if (read_arr(fd, arr_p) != 0)
        die("Error reading the file.\nfd:  %p\narr: %p\n", fd, arr_p);

    print_arr(arr_p);
    solve(arr_p);

    printf(NFCOL "\n| | | | | | | | | | | | | | | | | | |\n"
                 "v v v v v v v v v v v v v v v v v v v\n\n" NORM);
    print_arr(arr_p);

    fclose(fd);
    return 0;
}

