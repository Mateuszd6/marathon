# TODO: Ask if this makefile is OK; are auto dependencies allowed, etc.
CC=gcc
CFLAGS=-Wall -Wextra -std=c11
DEBUG_FLAGS=-g -O0 -DDEBUG
RELEASE_FLAGS=-O2

# Find all target .o files based on .c files.
OBJECTS=$(shell for i in *.c; do echo "$${i%.c}.o" ; done)

.PHONY: all release debug clean post_hook

# TODO: all: above the debug: !!!
debug: CFLAGS+=$(DEBUG_FLAGS)
debug: all

release: CFLAGS+=$(RELEASE_FLAGS)
release: all

all: bin post_hook

bin: $(OBJECTS)
	@mkdir -p bin/
	$(CC) $(OBJECTS) -o bin/program

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

# No trash in the working directory; remove all .dep files after compilation.
post_hook:
	@-rm *.dep
