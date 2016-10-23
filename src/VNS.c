/**
 * \file VNS.c
 * \brief implémentation de fonctions pour la méthaheuristique VNS et VND
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "VNS.h"
#include "Voisinage.h"

//------------------------------------------------------------------------------
void rechercheVND(Solution* sol) {

    int k = 1;
    int ameliore;

    constructionGloutonne(sol);

    printf("z initial : %d\n", sol->z);

    while(k < 4) {

        ameliore = rechercheLocale(sol, k);

        if(ameliore) {
            k = 1;
        } else {
            k ++;
        }
    }
}


//------------------------------------------------------------------------------
void rechercheVNS(Solution* sol) {

    int k = 1;
    int nbIt = 0, nbItMax = 5000;

    FILE* sortie = fopen("../plot/out.res", "w");

    constructionGloutonne(sol);

    printf("z initial : %d\n", sol->z);

    Solution voisin;
    creerSolution(sol->pb, &voisin);

    while(nbIt < nbItMax) {

        k = 1;

        while(k < 3) {

            // on part de la solution actuelle qui est recopiée
            copierSolution(sol, &voisin);

            // choix d'un voisin aléatoire
            int rea = voisinAlea(&voisin, k);

            // recherche locale sur ce voisin
            if(rea) {
                int initial = voisin.z;
                rechercheLocale(&voisin, k);
                fprintf(sortie, "%d %d %d %d\n", nbIt, initial, voisin.z, sol->z);
            }

            if(rea && voisin.z > sol->z) {
                copierSolution(&voisin, sol);
                k = 1;
                nbItMax = nbIt + 5000; // critère d'arrêt : pas d'amélioration depuis une certain nombre d'itération
            } else {
                k ++;
            }
        }

        nbIt ++;
    }

    detruireSolution(&voisin);

    fclose(sortie);

}


//------------------------------------------------------------------------------
int rechercheLocale(Solution* sol, int k) {

    int amelioration = 0;
    int ameliore = 0;

    do {

        switch(k) {
            case 1:
                ameliore = echange01(sol);
                break;
            case 2:
                ameliore = echange11(sol);
                break;
            case 3:
                ameliore = echange12(sol);
                break;
            default:
                break;
        }

        if(!amelioration && ameliore) {
            amelioration = 1;
        }

    } while(ameliore);

    return amelioration;
}
