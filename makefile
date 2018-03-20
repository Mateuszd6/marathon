# TODO: Ask if this makefile is OK; are auto dependencies allowed, etc.
CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
DEBUG_FLAGS=-Wall -Wextra -Wshadow -std=c11 -g -Og -DDEBUG
# Find all target .o files based on .c files.
OBJECTS=$(shell for i in *.c; do echo "$${i%.c}.o" ; done)

.PHONY: all debug clean post_hook

# TODO: all: above the debug: !!!
debug: CFLAGS=$(DEBUG_FLAGS)
debug: all
debug: post_hook

all: $(OBJECTS)
	@mkdir -p bin/
	$(CC) $(OBJECTS) -o bin/program
all: post_hook

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f bin/program

# Auto dependencies generator
# Make a .dep file for every .c file using CC -MM
%.dep : %.c
	@$(CC) -MM $(CFLAGS) $< > $@
include $(OBJECTS:.o=.dep)

# No trash in the working directory.
# Removes all .dep files once compilation is finished.
post_hook:
	@-rm *.dep
