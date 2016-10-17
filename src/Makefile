.PHONY: clean
CC = gcc
CFLAGS = -std=c99 -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextra

Probleme.o: Probleme.h Probleme.c
	$(CC) $(CFLAGS) Probleme.c -c

Solution.o: Solution.h Solution.c
	$(CC) $(CFLAGS) Solution.c -c

Voisinage.o: Voisinage.h Voisinage.c
	$(CC) $(CFLAGS) Voisinage.c -c

main: Probleme.o Solution.o Voisinage.o main.c
	$(CC) $(CFLAGS) main.c Probleme.o Solution.o Voisinage.o -o main

clean:
	@rm -rf *.o main
