/**
 * \file EliteSet.h
 * \brief structure de donnée contenant un ensemble de bonnes solutions
 */

#ifndef ELITE_SET_H
#define ELITE_SET_H

#include "Solution.h"

typedef struct {
    Solution solution; // une solution elite
    struct maillon *suiv; // le maillon suivant
} maillon;

typedef struct {
    maillon* premier;
    int taille;
} EliteSet;

/**
 * \brief création d'un ensemble de solution à envoyer dans le path relinking
 * \param l'ensemble à créer
 */
void creerEliteSet(EliteSet* set);

/**
 * \brief destruction de l'ensemble élite
 */
void detruireEliteSet(EliteSet* set);

/**
 * \brief ajout d'une solution élite dans l'ensemble
 * \param set l'ensemble à modier
 * \param sol la solution à ajouter
 */
void ajouterSolution(EliteSet* set, Solution* sol);


/**
 * \brief pioche aléatoirement une solution dans la liste élite
 * \param set la liste élite
 * \param sol la solution tirée
 */
void eliteRndm(EliteSet* set, Solution* sol);


#endif // ELITE_SET_H
