
# Wanted to try compiling with different object files

CC=gcc
CFLAGS=-Wall
LDFLAGS=-lncurses
OBJS=obj/main.o obj/interface.o obj/misc.o
SRCS=$(patsubst %.o, %.c, $(OBJS))
BIN=sudoku.out

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): obj/%.o : src/%.c obj
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

# obj folder
obj:
	mkdir -p obj

run: $(BIN)
	./$<

clean:
	rm -f $(OBJS)
	rm -f $(BIN)
