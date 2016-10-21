/**
 * \file VNS.H
 * \brief définitions de fonctions pour la méthaheuristique VNS et VND
 */

#ifndef VNS_H
#define VNS_H

#include "Solution.h"

/**
 * \brief applique le principe de la métaheuristique VND pour trouver une bonne solution
 * \param sol la meilleure solution retournée
 */
void rechercheVND(Solution* sol);


/**
 * \brief effectue une recherche locale de sol à partir d'un voisinage
 * \param sol la solution à améliorer
 * \param k le voisinage utilisé
 * \return vrai ssi une amélioration de la solution a été trouvée
 */
int rechercheLocale(Solution* sol, int k);

#endif // VNS_H
