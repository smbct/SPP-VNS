/**
 * \file Solution.c
 * \brief implémentation des fonctions pour manipuler un problème
 */ 

#include <stdlib.h>
#include "Solution.h"

//------------------------------------------------------------------------------
void initialiserZ(Solution* sol) {

    sol->z = 0;

    for(int indVar = 0; indVar < sol->pb->nbVar; indVar ++) {
        if(sol->valeur[indVar] != 0) {
            sol->z += sol->pb->cout[indVar];
        }
    }

}

//------------------------------------------------------------------------------
void initialiserSommeCtr(Solution* sol) {

    for(int indCtr = 0; indCtr < sol->pb->nbCtr; indCtr++) {

        // intialisation de la somme à 0
        sol->sommeCtr[indCtr] = 0;
        for(int indVar = 0; indVar < sol->pb->nbVar; indVar++) {
            if(sol->pb->contrainte[indCtr][indVar] == 1 && sol->valeur[indVar] == 1) {
                sol->sommeCtr[indCtr] ++;
            }
        }

    }

}

//------------------------------------------------------------------------------
void initialiserListeIndices(Solution* sol) {

    sol->nbVar0 = 0;
    sol->nbVar1 = 0;

    for(int i = 0; i < sol->pb->nbVar; i++) {
        if(sol->valeur[i] == 0) {
            sol->var0[sol->nbVar0] = i;
            sol->nbVar0 ++;
        } else {
            sol->var1[sol->nbVar1] = i;
            sol->nbVar1 ++;
        }
    }

}

//------------------------------------------------------------------------------
void constructionGloutonne(Solution* sol) {

    // ajouter plein de beau code ici

}


//------------------------------------------------------------------------------
void copierSolution(Solution* sol, Solution* copie) {

    for(int indVar = 0; indVar < sol->pb->nbVar; indVar++) {
        copie->valeur[indVar] = sol->valeur[indVar];
    }
    for(int indCtr = 0; indCtr < sol->pb->nbCtr; indCtr++) {
        copie->sommeCtr[indCtr] = sol->sommeCtr[indCtr];
    }
    for(int indVar0 = 0; indVar0 < sol->nbVar0; indVar0++) {
        copie->var0[indVar0] = sol->var0[indVar0];
    }
    for(int indVar1 = 0; indVar1 < sol->nbVar1; indVar1++) {
        copie->var1[indVar1] = sol->var1[indVar1];
    }
    copie->z = sol->z;
    copie->nbVar0 = sol->nbVar0;
    copie->nbVar1 = sol->nbVar1;

}

//------------------------------------------------------------------------------
void creerSolution(Probleme* pb, Solution* sol) {

    // allocation de tous les tableaux en mémoire
    sol->valeur = malloc(((long unsigned int)pb->nbVar)*sizeof(int));
    sol->sommeCtr = malloc(((long unsigned int)pb->nbCtr)*sizeof(int));
    sol->var0 = malloc(((long unsigned int)pb->nbVar)*sizeof(int));
    sol->var1 = malloc(((long unsigned int)pb->nbVar)*sizeof(int));
    sol->pb = pb; // initialisation du pointeur vers l'instance
}

//------------------------------------------------------------------------------
void detruireSolution(Solution* sol) {
    free(sol->valeur);
    free(sol->sommeCtr);
    free(sol->var0);
    free(sol->var1);
}
