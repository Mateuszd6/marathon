CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
# TODO: all: above the debug: !!!

debug: CFLAGS = -Wall -Wextra -Wshadow -std=c11 -Og -DDEBUG -g
debug: all

all: main.o linked_list.o
	$(CC) main.o linked_list.o -o bin/program

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

linked_list.o: linked_list.c
	$(CC) -c $(CFLAGS) linked_list.c -o linked_list.o

clean:
	-rm -f *.o
	-rm -f bin/program
