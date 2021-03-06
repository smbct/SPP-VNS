/**
 * \file Solution.c
 * \brief implémentation des fonctions pour manipuler un problème
 */

#include <stdlib.h>
#include <stdio.h>
#include "Solution.h"

//------------------------------------------------------------------------------
void resetSolution(Solution* sol) {

    sol->z = 0.;
    sol->nbVar0 = sol->pb->nbVar;
    sol->nbVar1 = 0;
    for(int i = 0; i < sol->pb->nbVar; i++) {
        sol->valeur[i] = 0;
        sol->var0[i] = i;
    }

    for(int i = 0; i < sol->pb->nbCtr; i++) {
        sol->sommeCtr[i] = 0;
    }

    sol->nbCtrVio = 0;

}

//------------------------------------------------------------------------------
void initialiserZ(Solution* sol) {

    sol->z = 0;

    for(int indVar = 0; indVar < sol->pb->nbVar; indVar ++) {
        if(sol->valeur[indVar] != 0) {
            sol->z += sol->pb->cout[indVar];
        }
    }

    sol->nbCtrVio = 0;

}

//------------------------------------------------------------------------------
void initialiserSommeCtr(Solution* sol) {

    sol->nbCtrVio = 0;
    for(int indCtr = 0; indCtr < sol->pb->nbCtr; indCtr++) {

        // intialisation de la somme à 0
        sol->sommeCtr[indCtr] = 0;
        for(int indVar = 0; indVar < sol->pb->nbVar; indVar++) {
            if(sol->pb->contrainte[indCtr][indVar] == 1 && sol->valeur[indVar] == 1) {
                sol->sommeCtr[indCtr] ++;
            }
        }

        if(sol->sommeCtr[indCtr] > 1) {
            sol->nbCtrVio ++;
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
void majSommeCtr1(Solution* sol, int ind) {

    // mise à jour des sommes des contraintes
    for(int i = 0; i < sol->pb->nbCtrVar[ind]; i++) {
        int indCtr = sol->pb->ctrVar[ind][i];

        if(sol->pb->contrainte[indCtr][ind] == 1) {
            sol->sommeCtr[indCtr] ++;

            if(sol->sommeCtr[indCtr] == 2) { // la contrainte est désormais violée
                sol->nbCtrVio ++;
            }
        }
    }

}

//------------------------------------------------------------------------------
void majSommeCtr0(Solution* sol, int ind) {

    // mise à jour des sommes des contraintes
    for(int i = 0; i < sol->pb->nbCtrVar[ind]; i++) {
        int indCtr = sol->pb->ctrVar[ind][i];
        if(sol->pb->contrainte[indCtr][ind] == 1) {
            sol->sommeCtr[indCtr] --;

            // la contrainte était peut-être violée
            if(sol->sommeCtr[indCtr] == 1) {
                sol->nbCtrVio --;
            }
        }
    }
}

//------------------------------------------------------------------------------
void constructionGloutonne(Solution* sol) {

    // initialisation des solutions
    for(int indVar = 0; indVar < sol->pb->nbVar; indVar ++) {
        sol->valeur[indVar] = 0;
    }

    // tableau des variables qui ne peuvent plus être sélectionnée
    int* inutilisable = malloc((long unsigned int)sol->pb->nbVar*sizeof(int));
    for(int i = 0; i < sol->pb->nbVar; i++) {
        inutilisable[i] = 0;
    }

    int nbVarRestant = sol->pb->nbVar;

    // des objets sont ajoutés à la solution tant que possible
    while(nbVarRestant > 0) {

        // recherche de l'utilité max
        double uMax = -1;
        int indMax = -1;

        for(int indVar = 0; indVar < sol->pb->nbVar; indVar++) {
            if(!inutilisable[indVar] && (uMax < 0 || sol->pb->utilite[indVar] > uMax)) {
                uMax = sol->pb->utilite[indVar];
                indMax = indVar;
            }
        }

        // affectation de la variable à 1
        sol->valeur[indMax] = 1;
        inutilisable[indMax] = 1; // la variable n'est plus sélectionnable
        nbVarRestant --;

        // suppression des variables ne pouvant plus être affectées à 1
        for(int indCtr = 0; indCtr < sol->pb->nbCtr; indCtr ++) {
            if(sol->pb->contrainte[indCtr][indMax] == 1) {
                // parcours des variables apparaissant dans la contrainte
                for(int indVar = 0; indVar < sol->pb->nbVar; indVar ++) {
                    if(sol->pb->contrainte[indCtr][indVar] == 1 && !inutilisable[indVar]) {
                        inutilisable[indVar] = 1;
                        nbVarRestant --;
                    }
                }
            }
        }
    }

    free(inutilisable);

    initialiserZ(sol);
    initialiserListeIndices(sol);
    initialiserSommeCtr(sol);
}


//------------------------------------------------------------------------------
void afficherSolution(Solution* sol) {

    for(int i = 0; i < sol->pb->nbVar; i++) {
        printf("%d", sol->valeur[i]);
        if(i < sol->pb->nbVar-1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
    printf("\nz = %d\n", sol->z);

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
    copie->pb = sol->pb;
    copie->nbCtrVio = sol->nbCtrVio;

}

//------------------------------------------------------------------------------
void creerSolution(Probleme* pb, Solution* sol) {

    // allocation de tous les tableaux en mémoire
    sol->valeur = malloc(((long unsigned int)pb->nbVar)*sizeof(int));
    sol->sommeCtr = malloc(((long unsigned int)pb->nbCtr)*sizeof(int));
    sol->var0 = malloc(((long unsigned int)pb->nbVar)*sizeof(int));
    sol->var1 = malloc(((long unsigned int)pb->nbVar)*sizeof(int));
    sol->pb = pb; // initialisation du pointeur vers l'instance
    sol->nbCtrVio = 0;
}

//------------------------------------------------------------------------------
void reconstruireSolution(Solution* sol) {

    while(sol->nbCtrVio > 0) {

        int varMin = -1;
        // enlever la variable qui a la plus petite utilité
        for(int indCtr = 0; indCtr < sol->pb->nbCtr; indCtr++) {

            // si c'est une des contraintes violées, la variable à réaffecter est recherchée
            if(sol->sommeCtr[indCtr] > 1) {

                // parcours de chaque variable
                for(int var = 0; var < sol->pb->nbVar; var++) {
                    if(sol->valeur[var] == 1 && sol->pb->contrainte[indCtr][var] == 1) {
                        // mise à jour de l'utilité minimale trouvée
                        if(varMin == -1 || sol->pb->utilite[varMin] < sol->pb->utilite[var]) {
                            varMin = var;
                        }
                    }
                }
            }
        }

        // mise à jour de la solution et des indices des variables à 1 et 0
        sol->valeur[varMin] = 0;

        int ind1Var = 0, trouve = 0;
        while(!trouve) {
            if(sol->var1[ind1Var] == varMin) {
                trouve = 1;
            } else {
                ind1Var ++;
            }
        }
        sol->var0[sol->nbVar0] = varMin;
        sol->nbVar0 ++;
        sol->nbVar1 --;
        sol->var1[ind1Var] = sol->var1[sol->nbVar1];

        // mise à jour de la valeur de la fonction objectif
        sol->z -= sol->pb->cout[varMin];

        // mise à jour de la somme des contraintes
        for(int indCtr = 0; indCtr < sol->pb->nbCtr; indCtr++) {

            if(sol->pb->contrainte[indCtr][varMin] == 1 && sol->pb->contrainte[indCtr][varMin] == 1) {
                // la contrainte doit être retirée de la liste des contraintes à rétablir
                if(sol->sommeCtr[indCtr] == 2) {
                    sol->nbCtrVio --;
                }
                sol->sommeCtr[indCtr] --;
            }
        }
    }
}

//------------------------------------------------------------------------------
void constructionGloutonneInverse(Solution* sol) {

    // initialisation des solutions
    for(int indVar = 0; indVar < sol->pb->nbVar; indVar ++) {
        sol->valeur[indVar] = 0;
    }

    // calcule des utilité
    double* utilite = malloc(((long unsigned int)sol->pb->nbVar)*sizeof(double));

    for(int indVar = 0; indVar < sol->pb->nbVar; indVar ++) {
         utilite[indVar] = calculerUtilite(sol->pb, indVar);
    }

    int nbVarRestant = sol->pb->nbVar;

    // des objets sont ajoutés à la solution tant que possible
    while(nbVarRestant > 0) {

        // recherche de l'utilité max
        double uMax = -1;
        int indMax = -1;

        // les utiliés à -1 sont celles pour lesquelles la variable ne peut plus être ajoutée
        for(int indVar = 0; indVar < sol->pb->nbVar; indVar++) {
            if( (uMax == -1 && utilite[indVar] >= 0) || utilite[indVar] < uMax) {
                uMax = utilite[indVar];
                indMax = indVar;
            }
        }

        // affectation de la variable à 1
        sol->valeur[indMax] = 1;
        utilite[indMax] = -1; // la variable n'est plus sélectionnable
        nbVarRestant --;

        // suppression des variables ne pouvant plus être affectées à 1
        for(int indCtr = 0; indCtr < sol->pb->nbCtr; indCtr ++) {
            if(sol->pb->contrainte[indCtr][indMax] == 1) {
                // parcours des variables apparaissant dans la contrainte
                for(int indVar = 0; indVar < sol->pb->nbVar; indVar ++) {
                    if(sol->pb->contrainte[indCtr][indVar] == 1 && utilite[indVar] >= 0) {
                        utilite[indVar] = -1;
                        nbVarRestant --;
                    }
                }
            }
        }
    }

    free(utilite);

    initialiserZ(sol);
    initialiserListeIndices(sol);
    initialiserSommeCtr(sol);
}

//------------------------------------------------------------------------------
void passerVariable1(Solution* sol, int indice) {

    // màj de la valeur de l'objectif
    sol->z += sol->pb->cout[sol->var0[indice]];

    // màj de la valeur de la variable
    sol->valeur[sol->var0[indice]] = 1;

    // màj des contraintes
    majSommeCtr1(sol, sol->var0[indice]);

    // màj des tableaux d'indice de variables à 0 et à 1
    sol->var1[sol->nbVar1] = sol->var0[indice];
    sol->nbVar1 ++;
    sol->nbVar0 --;
    sol->var0[indice] = sol->var0[sol->nbVar0];

}

//------------------------------------------------------------------------------
void passerVariable0(Solution* sol, int indice) {

    // màj de la valeur de l'objectif
    sol->z -= sol->pb->cout[sol->var1[indice]];

    // màj de la valeur de la variable
    sol->valeur[sol->var1[indice]] = 0;

    // màj des contraintes
    majSommeCtr0(sol, sol->var1[indice]);

    // màj des tableaux d'indice de variables à 0 et à 1
    sol->var0[sol->nbVar0] = sol->var1[indice];
    sol->nbVar0 ++;
    sol->nbVar1 --;
    sol->var1[indice] = sol->var1[sol->nbVar1];

}

//------------------------------------------------------------------------------
void detruireSolution(Solution* sol) {
    free(sol->valeur);
    free(sol->sommeCtr);
    free(sol->var0);
    free(sol->var1);
}
