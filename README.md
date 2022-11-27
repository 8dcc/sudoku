# Sudoku
**Simple sudoku for the terminal using ncurses and written in C.**

### Compiling
Simply run:
```bash
git clone https://github.com/r4v10l1/sudoku
cd sudoku

# For the game itself
make sudoku.out

# From a simple text solver
make solver.out

# Or both at once
make
```

### Using the solver
The solver program will try to read from the filename specified as argument, or stdin if none. The program checks if stdin is a piped file or command output (only on linux) and prints the usage if not.
```console
$ ./solver.out filename.txt
...

$ ./solver.out < filename.txt
...

$ cat filename.txt | ./solver.out
...

$ ./solver.out      # Will only check for pipes on linux
Usage:
   ./solver.out file.txt
   ./solver.out < file.txt

```
