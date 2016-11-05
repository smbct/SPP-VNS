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


    FILE* sortie = fopen("plot/out.res", "w");
    constructionGloutonne(sol);

    printf("z initial : %d\n", sol->z);

    int nbIter = 1;
    while(k <= 3) {

        ameliore = rechercheLocale(sol, k);
        fprintf(sortie, "%d %d %d %d\n", nbIter, sol->z, sol->z, sol->z);

        // printf("nbIter : %d, z : %d\n", nbIter, sol->z);

        if(ameliore) {
            k = 1;
            printf("nouveau z : %d\n", sol->z);
        } else {
            k ++;
        }
        nbIter ++;
    }

    fclose(sortie);
}


//------------------------------------------------------------------------------
void rechercheVNS(Solution* sol) {

    int k = 1;
    int nbIt = 0, nbItMax = 100;

    FILE* sortie = fopen("plot/out.res", "w");

    constructionGloutonne(sol);

    printf("z initial : %d\n", sol->z);

    Solution voisin;
    creerSolution(sol->pb, &voisin);

    while(nbIt < nbItMax) {

        k = 1;²

        while(k <= 3) {

            // printf("nbIt : %d\n", nbIt);

            // on part de la solution actuelle qui est recopiée
            copierSolution(sol, &voisin);

            // choix d'un voisin aléatoire
            int rea = voisinAlea(&voisin, k);
            int initial = voisin.z;

            // recherche locale sur ce voisin
            if(rea) {
                rechercheLocale(&voisin, k);
            }

            if(rea && voisin.z > sol->z) {
                copierSolution(&voisin, sol);
                k = 1;
                nbItMax = nbIt + 100; // critère d'arrêt : pas d'amélioration depuis une certain nombre d'itération
                printf("z : %d\n", sol->z);
            } else {
                k ++;
            }

            if(rea) {
                fprintf(sortie, "%d %d %d %d\n", nbIt, initial, voisin.z, sol->z);
            }
            nbIt ++;
        }

        // printf("zMax : %d, nbIt : %d\n", sol->z, nbIt);

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

        /*switch(k) {
            case 1:
                ameliore = kpGenerique(0, 1, sol);
                break;
            case 2:
                ameliore = kpGenerique(1, 1, sol);
                break;
            case 3:
                ameliore = kpGenerique(1, 2, sol);
                break;
            default:
                break;
        }*/

        if(!amelioration && ameliore) {
            amelioration = 1;
        }

    } while(ameliore);

    return amelioration;
}
