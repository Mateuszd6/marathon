CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
DEBUG_FLAGS=-Wall -Wextra -Wshadow -std=c11 -g -Og -DDEBUG

# Find all target .o files based on .c files.
OBJECTS=$(shell for i in *.c; do echo "$${i%.c}.o" ; done)

.PHONY: all debug clean

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

linked_list.o: linked_list.c linked_list.h
main.o: main.c linked_list.h tree.h
tree.o: tree.c linked_list.h tree.h
