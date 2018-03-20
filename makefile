CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
DEBUG_FLAGS=-Wall -Wextra -Wshadow -std=c11 -g -Og -DDEBUG

OBJECTS=tree.o main.o linked_list.o

.PHONY: all clean

# TODO: all: above the debug: !!!
debug: CFLAGS=$(DEBUG_FLAGS)
debug: all

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o bin/program

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f bin/program
