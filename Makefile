
# Wanted to try compiling with different object files

CC=gcc
CFLAGS=-Wall
LDFLAGS=-lncurses

GAME_OBJS=obj/game/main.o obj/game/interface.o obj/game/misc.o
GAME_BIN=sudoku.out

SOLVER_OBJS=obj/solver/main.o obj/solver/sudoku.o obj/solver/misc.o
SOLVER_BIN=solver.out

all: $(GAME_BIN) $(SOLVER_BIN)

run: $(GAME_BIN)
	./$<

# obj folders
obj:
	mkdir -p obj/game
	mkdir -p obj/solver

# -------------------------------------------
# Game

$(GAME_BIN): $(GAME_OBJS)
	$(CC) -o $@ $(GAME_OBJS) $(LDFLAGS)

$(GAME_OBJS): obj/game/%.o : src/game/%.c obj
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

# -------------------------------------------
# Solver

$(SOLVER_BIN): $(SOLVER_OBJS)
	$(CC) -o $@ $(SOLVER_OBJS)

$(SOLVER_OBJS): obj/solver/%.o : src/solver/%.c obj
	$(CC) $(CFLAGS) -c -o $@ $<

# -------------------------------------------

clean:
	rm -f $(GAME_OBJS)
	rm -f $(GAME_BIN)
	rm -f $(SOLVER_OBJS)
	rm -f $(SOLVER_BIN)
