/**
 * \file Probleme.c
 * \brief implémentation des fonctions pour manipuler les instance
 */

#include <stdio.h>
#include <stdlib.h>
#include "Probleme.h"

//------------------------------------------------------------------------------
void chargerProbleme(char* nom, Probleme *probleme) {

    FILE* fichier = fopen(nom, "r");

    if(fichier) {

        // lecture du nombre de contrainte et du nombre de variable
        int res = fscanf(fichier, "%d", &probleme->nbCtr);
        res = fscanf(fichier, "%d", &probleme->nbVar);

        // allocation de la mémoire du problème
        probleme->cout = malloc(((long unsigned int)probleme->nbVar)*sizeof(int));
        probleme->contrainte = malloc(((long unsigned int)probleme->nbCtr)*sizeof(int*));

        for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr++) {
            probleme->contrainte[indCtr] = malloc(((long unsigned int)probleme->nbVar)*sizeof(int));
        }

        // initialisation des contraintes
        for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr ++) {
            for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
                probleme->contrainte[indCtr][indVar] = 0;
            }
        }

        // lecture du vecteur des coûts
        for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
            res = fscanf(fichier, "%d", &probleme->cout[indVar]);
        }

        // lecture des contraintes
        for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr ++) {

            // nombre d'élément non nul dans la contraite
            int nbElem = 0;
            res = fscanf(fichier, "%d", &nbElem);

            // indices des éléments non nuls
            for(int i = 0; i < nbElem; i++) {
                int indice;
                res = fscanf(fichier, "%d", &indice);
                probleme->contrainte[indCtr][indice-1] = 1;
            }
        }


        // initialisation des listes d'association variable/contrainte
        probleme->nbCtrVar = malloc(((long unsigned int)probleme->nbVar)*sizeof(int));
        for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
            probleme->nbCtrVar[indVar] = 0;
        }
        // calcul du nombre de contrainte dans lesquelles apparait chaque variable
        for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr ++) {
            for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
                if(probleme->contrainte[indCtr][indVar]) {
                    probleme->nbCtrVar[indVar] ++;
                }
            }
        }

        // alocation des tableaux de liste
        probleme->ctrVar = malloc(((long unsigned int)probleme->nbVar)*sizeof(int*));
        for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
            probleme->ctrVar[indVar] = malloc(((long unsigned int)probleme->nbCtrVar[indVar])*sizeof(int));
        }

        // remplissage des tableaux
        for(int indVar = 0; indVar < probleme->nbVar; indVar ++)  {
            int ind = 0;
            // pour chaque variables les contraintes associées sont répertoriées
            for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr ++) {
                if(probleme->contrainte[indCtr][indVar]) {
                    probleme->ctrVar[indVar][ind] = indCtr;
                    ind ++;
                }
            }
        }

        // initialisation du tableau d'utilité
        probleme->utilite = malloc((long unsigned int)probleme->nbVar*sizeof(double));
        for(int var = 0; var < probleme->nbVar; var++) {
            probleme->utilite[var] = calculerUtilite(probleme, var);
        }

        /*printf("liste var - ctr : \n");
        for(int i = 0; i < probleme->nbVar; i++) {
            printf("%d : ", i);
            for(int j = 0; j < probleme->nbCtrVar[i]; j++) {
                printf("%d, ", probleme->ctrVar[i][j]);
            }
            printf("%d", probleme->nbCtrVar[i]);
            printf("\n");
        }*/


    } else {
        printf("Erreur, impossible de charger l'instance.\n");
    }

}

//------------------------------------------------------------------------------
void detruireProbleme(Probleme* probleme) {

    // désallocation de la liste d'association entre variables et contraintes
    for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
        free(probleme->ctrVar[indVar]);
    }
    free(probleme->ctrVar);
    free(probleme->nbCtrVar);

    // désallocation des couts
    free(probleme->cout);

    // désallocation des contraintes
    for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr ++) {
        free(probleme->contrainte[indCtr]);
    }

    free(probleme->contrainte);

    free(probleme->utilite);
}

//------------------------------------------------------------------------------
void afficherProbleme(Probleme* probleme) {

    printf("max z = ");
    for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
        printf("%d ", probleme->cout[indVar]);
    }
    printf("\n\ns/c\n");

    for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr++) {
        for(int indVar = 0; indVar < probleme->nbVar; indVar++) {
            printf("%d ", probleme->contrainte[indCtr][indVar]);
        }
        printf("\n");
    }

}

//------------------------------------------------------------------------------
double calculerUtilite(Probleme* pb, int var) {
    int somme = 0;

    for(int i = 0; i < pb->nbCtr; i++) {
        if(pb->contrainte[i][var] == 1) {
            somme ++;
        }
    }

    return (double)pb->cout[var]/(double)somme;
}
