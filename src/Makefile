.PHONY: clean
CC = gcc
CFLAGS = -std=c99 -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextra -O3 -pg

Probleme.o: Probleme.h Probleme.c
	$(CC) $(CFLAGS) Probleme.c -c

Solution.o: Solution.h Solution.c
	$(CC) $(CFLAGS) Solution.c -c

Voisinage.o: Voisinage.h Voisinage.c
	$(CC) $(CFLAGS) Voisinage.c -c

VNS.o: VNS.h VNS.c
	$(CC) $(CFLAGS) VNS.c -c

main: Probleme.o Solution.o Voisinage.o main.c VNS.o
	$(CC) $(CFLAGS) main.c Probleme.o Solution.o Voisinage.o VNS.o -o main

glpkSolver: Probleme.o Solution.o glpkSolver.c
	$(CC) $(CFLAGS) glpkSolver.c Probleme.o Solution.o -lglpk -o glpkSolver

clean:
	@rm -rf *.o main
