# Sudoku
**Simple sudoku for the terminal using ncurses and written in C.**

![ss1](https://user-images.githubusercontent.com/29655971/227586109-f89fe2d0-bc6c-4d11-9c30-dd0d8eac2565.png)
![ss2](https://user-images.githubusercontent.com/29655971/227586117-b1c33200-f3c5-409d-b168-860b6a22002c.png)

### Building
Simply run:
```bash
git clone https://github.com/8dcc/sudoku
cd sudoku

# For the game itself
make sudoku.out

# From a simple text solver
make solver.out

# Or both at once
make
```

If you don't want the **game** to show colors (for ncurses) simply comment this line in [`src/game/globals.h`](https://github.com/8dcc/sudoku/blob/dabfb31086e15f60f837844885fabe533d98c2aa/src/game/globals.h#L5-L8).

If you don't want the **solver** program to show colors (for example for some windows terminals or for redirecting the output) simply comment this line in [`src/solver/defines.h`](https://github.com/8dcc/sudoku/blob/dabfb31086e15f60f837844885fabe533d98c2aa/src/solver/defines.h#L2-L7).

```c
/*
 * Comment this line if you don't want colors!
 * Will not print with colors if not defined, useful for redirecting the output of
 * the command.
 */
#define USE_COLOR
```

### Playing the game
Simply run:
```console
$ ./sudoku.out --help
Usage:
    ./sudoku.out            - Run with default difficulty.
    ./sudoku.out <number>   - Where number is the number of cells that are going to be filled. [1-80]
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
