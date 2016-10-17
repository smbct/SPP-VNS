#include <stdlib.h>
#include "Solution.h"

//------------------------------------------------------------------------------
void initialiserZ(Probleme* pb, Solution* sol) {

    sol->z = 0;

    for(int indVar = 0; indVar < pb->nbVar; indVar ++) {
        if(sol->valeur[indVar] != 0) {
            sol->z += pb->cout[indVar];
        }
    }

}

//------------------------------------------------------------------------------
void initialiserSommeCtr(Probleme* pb, Solution* sol) {

    for(int indCtr = 0; indCtr < pb->nbCtr; indCtr++) {

        // intialisation de la somme à 0
        sol->sommeCtr[indCtr] = 0;
        for(int indVar = 0; indVar < pb->nbVar; indVar++) {
            if(pb->contrainte[indCtr][indVar] == 1 && sol->valeur[indVar] == 1) {
                sol->sommeCtr[indCtr] ++;
            }
        }

    }

}

//------------------------------------------------------------------------------
void constructionGloutonne(Probleme* pb, Solution* sol) {

    // ajouter plein de beau code ici

}


//------------------------------------------------------------------------------
void copierSolution(Probleme* pb, Solution* sol, Solution* copie) {

    for(int indVar = 0; indVar < pb->nbVar; indVar++) {
        copie->valeur[indVar] = sol->valeur[indVar];
    }
    for(int indCtr = 0; indCtr < pb->nbCtr; indCtr++) {
        copie->sommeCtr[indCtr] = sol->sommeCtr[indCtr];
    }
    copie->z = sol->z;

}

//------------------------------------------------------------------------------
void creerSolution(Probleme* pb, Solution* sol) {
    sol->valeur = malloc(((long unsigned int)pb->nbVar)*sizeof(int));
    sol->sommeCtr = malloc(((long unsigned int)pb->nbCtr)*sizeof(int));
}

//------------------------------------------------------------------------------
void detruireSolution(Solution* sol) {
    free(sol->valeur);
    free(sol->sommeCtr);
}
