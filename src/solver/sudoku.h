
#ifndef SUDOKU_H_
#define SUDOKU_H_ 1

#include <stdbool.h>

void init_arr(int* arr);
int read_arr(FILE* fd, int* arr);
void print_arr(int* arr, bool* empty);
void solve(int* arr);

#endif /* SUDOKU_H_ */
