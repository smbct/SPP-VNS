/**
 * \file Voisinage.h
 * \brief définition de fonctions pour calculer le voisinage d'une solution
 */

#ifndef VOISINAGE_H
#define VOISINAGE_H

#include "Solution.h"


/**
 * \brief recherche une amélioration de la solution avec un 0/1 échange
 * \param sol la solution à améliorer
 * \return 1 ssi la solution a été améliorée
 */
int echange01(Solution* sol);

/**
 * \brief recherche une amélioration de la solution avec un 1/1 échange
 * \param sol la solution à améliorer
 * \return 1 ssi la solution a été améliorée
 */
int echange11(Solution* sol);

/**
 * \brief recherche une amélioration de la solution avec un 1/2 échange
 * \param sol la solution à améliorer
 * \return 1 ssi la solution a été améliorée
 */
int echange12(Solution* sol);

/**
 * \bref effectue un echange aléatoire entre deux variables
 * \param sol la solution sur laquelle faire le mouvement
 * \param k le nombre de variable à passer à 0
 * \param p le nombre de variable à passer à 1
 * \return 1 ssi la solution est réalisable
 */
int echangeAlea(Solution* sol, int k, int p);

/**
 * \brief met à jour la somme sur les contraintes pour la passage d'une variable de 0 à 1
 * \param sol la solution modifiée
 * \param ind l'indice de la variable à passer à 1
 * \return 1 ssi la solution est toujours réalisable
 */
int majSommeCtr1(Solution* sol, int ind);

/**
 * \brief met à jour la somme sur les contraintes pour la passage d'une variable de 1 à 0
 * \param sol la solution modifiée
 * \param ind l'indice de la variable à passer à 0
 * \return 1 ssi la solution est toujours réalisable
 */
int majSommeCtr0(Solution* sol, int ind);

/**
 * \brief echange deux valeurs
 * \param val1 pointeur vers la première valeur
 * \param val2 pointeur vers la deuxième valeur
 */
void echanger(int* val1, int* val2);

#endif
