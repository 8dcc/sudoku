
CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lncurses -ltinfo

GAME_OBJS=obj/game/main.o obj/game/sudoku.o obj/game/interface.o obj/game/misc.o
GAME_BIN=sudoku.out

SOLVER_OBJS=obj/solver/main.o obj/solver/sudoku.o obj/solver/misc.o
SOLVER_BIN=solver.out

#-------------------------------------------------------------------------------

.PHONY: all run clean

all: $(GAME_BIN) $(SOLVER_BIN)

run: $(GAME_BIN)
	./$<

clean:
	rm -f $(SOLVER_BIN) $(GAME_BIN)
	rm -f $(GAME_OBJS) $(SOLVER_OBJS)

#-------------------------------------------------------------------------------
# Game

$(GAME_BIN): $(GAME_OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(GAME_OBJS) $(LDFLAGS)

$(GAME_OBJS): obj/game/%.o : src/game/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

#-------------------------------------------------------------------------------
# Solver

$(SOLVER_BIN): $(SOLVER_OBJS)
	$(CC) -o $@ $(SOLVER_OBJS)

$(SOLVER_OBJS): obj/solver/%.o : src/solver/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
