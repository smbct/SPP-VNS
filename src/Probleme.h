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
    double* utilite; // utilité des variables
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

/**
 * \brief calcul de l'utilité d'une variable
 * \param pb l'instance du problème SPP
 * \param var l'indice de la variable dont on calcule l'utilité
 */
double calculerUtilite(Probleme* pb, int var);

#endif // PROBLEME_H
