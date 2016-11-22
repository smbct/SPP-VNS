.PHONY: clean
CC = gcc
CFLAGS = -std=c99 -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextra -O3 -Ofast #-pg

src/Probleme.o: src/Probleme.h src/Probleme.c
	$(CC) $(CFLAGS) src/Probleme.c -c -o src/Probleme.o

src/Solution.o: src/Solution.h src/Solution.c
	$(CC) $(CFLAGS) src/Solution.c -c -o src/Solution.o

src/Voisinage.o: src/Voisinage.h src/Voisinage.c
	$(CC) $(CFLAGS) src/Voisinage.c -c -o src/Voisinage.o

src/VNS.o: src/VNS.h src/VNS.c
	$(CC) $(CFLAGS) src/VNS.c -c -o src/VNS.o

main: src/Probleme.o src/Solution.o src/Voisinage.o src/main.c src/VNS.o
	$(CC) $(CFLAGS) src/main.c src/Probleme.o src/Solution.o src/Voisinage.o src/VNS.o -o main

glpkSolver: src/Probleme.o src/Solution.o src/glpkSolver.c
	$(CC) $(CFLAGS) src/glpkSolver.c src/Probleme.o src/Solution.o -lglpk -o glpkSolver

clean:
	@rm -rf src/*.o main
