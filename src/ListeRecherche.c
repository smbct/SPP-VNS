/**
 * \file ListeRecherche.c
 * \brief implémentation des fonctions de manip de la liste de recherche
 */

#include "ListeRecherche.h"

#include <stdlib.h>
#include <stdio.h>

//------------------------------------------------------------------------------
void initialiserListeRecherche(ListeRecherche* liste) {
    liste->dernier = NULL;
    liste->taille = 0;
}

//------------------------------------------------------------------------------
void ajouterSolutionListeRecherche(ListeRecherche* liste, Solution* sol) {
    MaillonRecherche* maillon = malloc(sizeof(MaillonRecherche));
    creerSolution(sol->pb, &maillon->sol);
    copierSolution(sol, &maillon->sol);
    maillon->nbIt = 0;
    maillon->prec = liste->dernier;
    liste->dernier = maillon;
    liste->taille ++;
}

//------------------------------------------------------------------------------
void retirerSolutionListeRecherche(ListeRecherche* liste) {

    MaillonRecherche* maillonSup = liste->dernier;
    liste->dernier = maillonSup->prec;
    detruireSolution(&maillonSup->sol);
    free(maillonSup);
    liste->taille --;
}
