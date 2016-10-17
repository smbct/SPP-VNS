#ifndef SOLUTION_H
#define SOLUTION_H

#include "Probleme.h"

// structure de donnée représentant une solution du problème de SPP
typedef struct {
    int* valeur; // affectations des variables du problème
    int z; // valeur de la fonction objectif
    int* sommeCtr; // valeurs des sommes des contraintes
} Solution;


void initialiserZ(Probleme* pb, Solution* sol);
void initialiserSommeCtr(Probleme* pb, Solution* sol);
void constructionGloutonne(Probleme* pb, Solution* sol); // construction d'une solution gloutonne

/**
 * \brief création d'une solution non initialisée en mémoire
 * \param pb l'instance du problème SPP
 * \sol la solution créée
 */
void creerSolution(Probleme* pb, Solution* sol);

/**
 * \brief copie d'une solution
 * \param pb l'instance du problème de SPP
 * \param sol la solution à copier
 * \param copie la copie de la solution
 */
void copierSolution(Probleme* pb, Solution* sol, Solution* copie);

void detruireSolution(Solution* sol);


#endif // SOLUTION_H
