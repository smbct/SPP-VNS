/**
 * \file EliteSet.c
 * \brief structure de donnée contenant un ensemble de bonnes solutions
 */

#include "EliteSet.h"

#include "Voisinage.h" // pour aleaBorne

#include <stdlib.h>
#include <stdio.h>

//------------------------------------------------------------------------------
void creerEliteSet(EliteSet* set) {
    set->premier = 0;
    set->taille = 0;
}

//------------------------------------------------------------------------------
void detruireEliteSet(EliteSet* set) {

    set->taille = 0;

    maillon* maille = set->premier;
    maillon* aSup;

    while(maille != 0) {
        aSup = maille;
        maille = maille->suiv;

        detruireSolution(&aSup->solution);
        free(aSup);
    }
}

//------------------------------------------------------------------------------
void ajouterSolution(EliteSet* set, Solution* sol) {

    maillon* nouveau = malloc(sizeof(maillon));
    creerSolution(sol->pb, &nouveau->solution);
    copierSolution(sol, &nouveau->solution);

    nouveau->suiv = set->premier;
    set->premier = nouveau;
    set->taille ++;

}


//------------------------------------------------------------------------------
void eliteRndm(EliteSet* set, Solution* sol) {

    int indice = aleaBorne(0, set->taille-1);

    maillon* mai = set->premier;
    for(int i = 0; i < indice; i++) {
        mai = mai->suiv;
    }

    copierSolution(&mai->solution, sol);
}
