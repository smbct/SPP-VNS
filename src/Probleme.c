#include <stdio.h>
#include <stdlib.h>
#include "Probleme.h"

//------------------------------------------------------------------------------
void chargerProbleme(char* nom, Probleme *probleme) {

    FILE* fichier = fopen(nom, "r");

    if(fichier) {

        // lecture du nombre de contrainte et du nombre de variable
        fscanf(fichier, "%d", &probleme->nbCtr);
        fscanf(fichier, "%d", &probleme->nbVar);

        // allocation de la mémoire du problème
        probleme->couts = malloc(((long unsigned int)probleme->nbVar)*sizeof(int));
        probleme->contraintes = malloc(((long unsigned int)probleme->nbCtr)*sizeof(int*));

        for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr++) {
            probleme->contraintes[indCtr] = malloc(((long unsigned int)probleme->nbVar)*sizeof(int));
        }

        // initialisation des contraintes
        for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr ++) {
            for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
                probleme->contraintes[indCtr][indVar] = 0;
            }
        }

        // lecture du vecteur des coûts
        for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
            fscanf(fichier, "%d", &probleme->couts[indVar]);
        }

        // lecture des contraintes
        for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr ++) {

            // nombre d'élément non nul dans la contraite
            int nbElem = 0;
            fscanf(fichier, "%d", &nbElem);

            // indices des éléments non nuls
            for(int i = 0; i < nbElem; i++) {
                int indice;
                fscanf(fichier, "%d", &indice);
                probleme->contraintes[indCtr][indice] = 1;
            }
        }

    } else {
        printf("Erreur, impossible de charger l'instance.\n");
    }

}

//------------------------------------------------------------------------------
void detruireProbleme(Probleme* probleme) {

    // désallocation des couts
    free(probleme->couts);

    // désallocation des contraites
    for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr ++) {
        free(probleme->contraintes[indCtr]);
    }

    free(probleme->contraintes);
}

//------------------------------------------------------------------------------
void afficherProbleme(Probleme* probleme) {

    printf("max z = ");
    for(int indVar = 0; indVar < probleme->nbVar; indVar ++) {
        printf("%d ", probleme->couts[indVar]);
    }
    printf("\n\ns/c\n");

    for(int indCtr = 0; indCtr < probleme->nbCtr; indCtr++) {
        for(int indVar = 0; indVar < probleme->nbVar; indVar++) {
            printf("%d ", probleme->contraintes[indCtr][indVar]);
        }
        printf("\n");
    }

}
