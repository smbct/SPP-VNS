/**
 * \file Solution.h
 * \brief définition d'une structure de donnée Solution et de fonctions pour la manipuler
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include "Probleme.h"

// structure de donnée représentant une solution du problème de SPP
typedef struct {
    int* valeur; // affectations des variables du problème
    int z; // valeur de la fonction objectif
    int* sommeCtr; // valeurs des sommes des contraintes
    int* var0; // liste des indices des variables à 0
    int* var1; // liste des indices des variables à 1
    int nbVar0; // nombre de variables à 0
    int nbVar1; // nombre de variable à 1
    Probleme* pb; // pointeur vers l'instance du problème
}  Solution;


/**
 * \brief initialisation de la valeur de la fonction objectif pour une solution
 * \param sol la soltion initialisée
 */
void initialiserZ(Solution* sol);

/**
 * \brief initialisation d'un vecteur somme des contraintes pour une solution
 * \param sol la solution initialisée
 */
void initialiserSommeCtr(Solution* sol);

/**
 * \brief initialisation de la liste des indices des variables à 0 et à 1
 * \param sol la solution initialisée
 */
void initialiserListeIndices(Solution* sol);

/**
 * \brief calcul de l'utilité d'une variable
 * \param pb l'instance du problème SPP
 * \param var l'indice de la variable dont on calcule l'utilité
 */
int calculerUtilite(Probleme* pb, int var);

/**
 * \brief construction d'une solution gloutonne pour le problème
 * \param sol la solution retournée
 */
void constructionGloutonne(Solution* sol);


/**
 * \brief affichage d'une solution sur la console
 * \param sol la solution à afficher
 */
void afficherSolution(Solution* sol);

/**
 * \brief création d'une solution non initialisée en mémoire
 * \param pb l'instance du problème SPP à laquelle correspond la solution
 * \sol la solution créée
 */
void creerSolution(Probleme* pb, Solution* sol);

/**
 * \brief copie d'une solution
 * \param sol la solution à copier
 * \param copie la copie de la solution
 */
void copierSolution(Solution* sol, Solution* copie);

/**
 * \brief désallocation mémoire d'une solution
 * \param sol la solution à désallouer
 */
void detruireSolution(Solution* sol);


#endif // SOLUTION_H
