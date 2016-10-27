/**
 * \file Probleme.h
 * \brief définition d'une structure de problème et de fonctions pour le manipuler
 */

#ifndef PROBLEME_H
#define PROBLEME_H

// structure de donnée contenant une instance du problème
typedef struct {
    int nbVar; // nombre de variable
    int nbCtr; // nombre de contrainte
    int* cout; // vecteur de coûts
    int** contrainte; // matrice des contraintes
    int* nbCtrVar; // nombre de contrainte dans laquelle apparait chaque variables
    int** ctrVar; // contraintes dans lesquelles chaque variable apparait
} Probleme;

/**
 * \brief chargement d'une instance à partir d'un fichier au format OR-library
 * \param nom le nom du fichier à charger
 * \param probleme l'instance à charger
 */
void chargerProbleme(char* nom, Probleme *probleme);

/**
 * \brief désallocation de l'instance en mémoire
 * \param probleme le problème à désallouer
 */
void detruireProbleme(Probleme* probleme);

/**
 * \brief affichage de l'instance dans la console
 * \param probleme l'instance à afficher
 */
void afficherProbleme(Probleme* probleme);

#endif // PROBLEME_H
