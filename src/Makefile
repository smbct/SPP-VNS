.PHONY: clean
CC = gcc
CFLAGS = -std=c99 -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextra

Probleme.o: Probleme.h Probleme.c
	$(CC) $(CFLAGS) Probleme.c -c

main: Probleme.o main.c
	$(CC) $(CFLAGS) main.c Probleme.o -o main

clean:
	@rm -rf *.o
