/**
 * \file Voisinage.h
 * \brief définition de fonctions pour calculer le voisinage d'une solution
 */

#ifndef VOISINAGE_H
#define VOISINAGE_H

#include "Solution.h"

/**
 * \brief enregistrement contenant les variables à échanger dans le mouvement
 */
typedef struct {

    int k; // nombre de variable qui ont été passées de 1 à 0
    int p; // nombre de variable qui ont été passées de 0 à 1

    int* vark; // indices des variables passées de 1 à 0
    int* varp; // indices des variables passées de 0 à 1

    int z; // valeur de la fonction objectif après cet échange
} kpEchange;

/**
 * \brief recherche une amélioration de la solution avec un 0/1 échange
 * \param sol la solution à améliorer
 * \param alea indique si la solution choisie est aléatoire ou non
 * \return 1 ssi la solution a été améliorée
 */
int echange01(Solution* sol);

/**
 * \brief pioche une solution aléatoire dans le voisinage 0/1
 * \param sol la solution aléatoire, si possible
 * \return 0 ssi aucune solution n'est réalisable
 */
int echange01Alea(Solution* sol);

/**
 * \brief recherche une amélioration de la solution avec un 1/1 échange
 * \param sol la solution à améliorer
 * \return 1 ssi la solution a été améliorée
 */
int echange11(Solution* sol);

/**
 * \brief pioche une solution aléatoire dans le voisinage 1/1
 * \param sol la solution aléatoire, si trouvée
 * \return 0 ssi aucune solution n'est réalisable
 */
int echange11Alea(Solution* sol);

/**
 * \brief recherche une amélioration de la solution avec un 1/2 échange
 * \param sol la solution à améliorer
 * \return 1 ssi la solution a été améliorée
 */
int echange12(Solution* sol);

/**
 * \brief pioche une solution aléatoire dans le voisinage 1/2
 * \param sol la solution aléatoire, si trouvée
 * \return 0 ssi aucune solution n'est réalisable
 */
int echange12Alea(Solution* sol);

/**
 * \bref effectue un echange aléatoire entre deux variables
 * \param sol la solution sur laquelle faire le mouvement
 * \param k le nombre de variable à passer à 0
 * \param p le nombre de variable à passer à 1
 * \return 1 ssi la solution est réalisable
 */
int echangeAlea(Solution* sol, int k, int p);

/**
 * \brief recherche un voisin aléatoire en fonction du niveau de voisinage
 * \param sol la solution actuelle, le voisin aléatoire s'il a été trouvé
 * \param k le niveau de voisinage choisi
 * \return 1 ssi un voisin réalisable a été trouvé
 */
int voisinAlea(Solution* sol, int k);

/**
 * \brief met à jour la somme sur les contraintes pour la passage d'une variable de 0 à 1
 * \param sol la solution modifiée
 * \param ind l'indice de la variable à passer à 1
 */
void majSommeCtr1(Solution* sol, int ind);

/**
 * \brief met à jour la somme sur les contraintes pour la passage d'une variable de 1 à 0
 * \param sol la solution modifiée
 * \param ind l'indice de la variable à passer à 0
 */
void majSommeCtr0(Solution* sol, int ind);

/**
 * \brief echange deux valeurs
 * \param val1 pointeur vers la première valeur
 * \param val2 pointeur vers la deuxième valeur
 */
void echanger(int* val1, int* val2);

/**
 * \brief choisi un nombre aléatoire entre min et max inclus
 * \param min la valeur minimale de l'intervalle
 * \param max la valeur maximale
 * \return le nombre aléatoire
 */
int aleaBorne(int min, int max);

void kpGeneriqueRec(int k, int p, kpEchange* actuel, Solution* sol, kpEchange* meilleur);

int kpGenerique(int k, int p, Solution* sol);

#endif
