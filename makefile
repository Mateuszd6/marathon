CC=gcc

DEBUG_FLAGS=-Wall -Wextra -std=c11 -g -O0 -DDEBUG
RELEASE_FLAGS=-Wall -Wextra -std=c11 -O2

# 'release' is a defaul target. To build with debug 'make debug' must be called.
CFLAGS=$(RELEASE_FLAGS)

EXECUTABLE_NAME=marathon

# Find all target .o files based on .c files.
OBJECTS=$(shell for i in *.c; do echo "$${i%.c}.o" ; done)

.PHONY: all debug clean post_hook

all: $(EXECUTABLE_NAME) post_hook

debug: CFLAGS=$(DEBUG_FLAGS)
debug: all

$(EXECUTABLE_NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE_NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean: post_hook
	@-rm -f *.o
	@-rm -f $(EXECUTABLE_NAME)

# Make a .dep file for every .c file using $(CC) -MM. This will auto-generate
# file dependencies.
%.dep : %.c
	@$(CC) -MM $(CFLAGS) $< > $@
include $(OBJECTS:.o=.dep)

# No trash in the working directory; remove all .dep files after compilation.
# The downside of generationg .dep fiels is then [post_hook] must be called
# after any command now, and may confuse other users.
post_hook:
	@-rm *.dep
