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
 * \brief applique le principe de la métaheuristique VNS pour trouver une bonne solution
 * \param sol la meilleurs solution trouvée
 * \param option 1 : plusieurs essais en aléatoire, 2 : un essais puis reconstruction, 3 : génération du voisinage puis choix aléatoire
 */
void rechercheVNS(Solution* sol, int option);

/**
 * \brief effectue une recherche locale de sol à partir d'un voisinage
 * \param sol la solution à améliorer
 * \param k le voisinage utilisé
 * \return vrai ssi une amélioration de la solution a été trouvée
 */
int rechercheLocale(Solution* sol, int k);

/**
 * TODO
 */
void path_relinking(Solution* best , Solution* worst);

#endif // VNS_H
