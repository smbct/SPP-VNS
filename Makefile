.PHONY: clean
CC = gcc
CFLAGS = -std=c99 -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextra -O3 -Ofast #-pg

OBJECTS_FILES = src/ListeRecherche.o src/Probleme.o src/Solution.o src/Voisinage.o src/VNS.o src/EliteSet.o src/grasp.o

# Compilation des fichiers object
%.o: %.c %.h
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

main: src/main.c $(OBJECTS_FILES)
	$(CC) $(CFLAGS) src/main.c $(OBJECTS_FILES) -o main

glpkSolver: src/Probleme.o src/Solution.o src/glpkSolver.c
	$(CC) $(CFLAGS) src/glpkSolver.c src/Probleme.o src/Solution.o -lglpk -o glpkSolver

clean:
	@rm -rf src/*.o main
