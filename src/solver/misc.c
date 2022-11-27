
#include <stdio.h>     // For vfprintf
#include <stdlib.h>    // For exit
#include <stdarg.h>    // For va_list

void die(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);

    vfprintf(stderr, fmt, va);

    va_end(va);
    exit(1);
}
