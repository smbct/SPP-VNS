/**
 * \file ListeRecherche.h
 * \brief définition d'une structure de liste chaînée pour la recherche dans l'arbre
 */

#ifndef LISTE_RECHERCHE_H
#define LISTE_RECHERCHE_H

#include "Solution.h"

typedef struct MaillonRecherche {

    struct MaillonRecherche* prec;
    Solution sol; // Solution courante
    int nbIt; // nombre d'itération passé sur cette solution

} MaillonRecherche;

typedef struct ListeRecherche {

    MaillonRecherche* dernier;
    int taille;

} ListeRecherche;



void initialiserListeRecherche(ListeRecherche* liste);

void ajouterSolutionListeRecherche(ListeRecherche* liste, Solution* sol);

void retirerSolutionListeRecherche(ListeRecherche* liste);

#endif // LISTE_RECHERCHE_H
