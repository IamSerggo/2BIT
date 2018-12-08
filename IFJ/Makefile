CC=gcc
CFLAGS= -std=c99 -Wall -pedantic-errors -g

all: main

main: main.o parser.o precedence_sa.o scanner.o stack.o error.o symtable.o generator.o linked_list.o queue.o
	$(CC) $^ -o $@ $(CFLAGS)

main.o: main.c
	$(CC) -c $^ -o $@ $(CFLAGS)

parser.o: parser.c
	$(CC) -c $^ -o $@ $(CFLAGS)

precedence_sa.o: precedence_sa.c
	$(CC) -c $^ -o $@ $(CFLAGS)

scanner.o: scanner.c
	$(CC) -c $^ -o $@ $(CFLAGS)

stack.o: stack.c
	$(CC) -c $^ -o $@ $(CFLAGS)

error.o: error.c
	$(CC) -c $^ -o $@ $(CFLAGS)

symtable.o: symtable.c
	$(CC) -c $^ -o $@ $(CFLAGS)

generator.o: generator.c
	$(CC) -c $^ -o $@ $(CFLAGS)

linked_list.o: linked_list.c
	$(CC) -c $^ -o $@ $(CFLAGS)

queue.o: queue.c
	$(CC) -c $^ -o $@ $(CFLAGS)

tests: tests.sh

pack:
	zip xbabic09 *.c *.h Makefile dokumentace.pdf rozdeleni rozsireni

clean:
	rm -f *.o main tests