
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>    // For va_list

#ifdef __unix__
#include <unistd.h>    // isatty()
#endif

void die(const char* fmt, ...);

int main(int argc, char** argv) {
    FILE* fd = stdin;

    if (argc > 1) {
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

    printf("I am the sudoku solver!\n");

    fclose(fd);
    return 0;
}

void die(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);

    vfprintf(stderr, fmt, va);

    va_end(va);
    exit(1);
}
