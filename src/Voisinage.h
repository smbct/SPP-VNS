/**
 * \file Voisinage.h
 * \brief définition de fonctions pour calculer le voisinage d'une solution
 */

#ifndef VOISINAGE_H
#define VOISINAGE_H

#include "Solution.h"

/**
 * \brief recherche une amélioration de la solution avec un 1/1 échange
 * \param sol la solution à améliorer
 * \return 1 ssi la solution a été améliorée
 */
int echange11(Solution* sol);

#endif
