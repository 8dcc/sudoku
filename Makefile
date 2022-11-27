
# Wanted to try compiling with different object files

CC=gcc
CFLAGS=-Wall
LDFLAGS=-lncurses

GAME_OBJS=obj/game/main.o obj/game/interface.o obj/game/misc.o
GAME_SRCS=$(patsubst %.o, %.c, $(OBJS))
GAME_BIN=sudoku.out

SOLVER_OBJS=obj/solver/main.o
SOLVER_SRCS=$(patsubst %.o, %.c, $(SOLVER_OBJS))
SOLVER_BIN=solver.out

all: $(GAME_BIN) $(SOLVER_BIN)

run: $(GAME_BIN)
	./$<

# -------------------------------------------
# Game

$(GAME_BIN): $(GAME_OBJS)
	$(CC) -o $@ $(GAME_OBJS) $(LDFLAGS)

$(GAME_OBJS): obj/game/%.o : src/game/%.c obj/game
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

# obj folder
obj/game:
	mkdir -p obj/game

# -------------------------------------------
# Solver

$(SOLVER_BIN): $(SOLVER_OBJS)
	$(CC) -o $@ $(SOLVER_OBJS)

$(SOLVER_OBJS): obj/solver/%.o : src/solver/%.c obj/solver
	$(CC) $(CFLAGS) -c -o $@ $<

obj/solver:
	mkdir -p obj/solver

# -------------------------------------------

clean:
	rm -f $(OBJS)
	rm -f $(BIN)
