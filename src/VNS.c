/**
 * \file VNS.c
 * \brief implémentation de fonctions pour la méthaheuristique VNS et VND
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "VNS.h"
#include "Voisinage.h"
#include "EliteSet.h"

//------------------------------------------------------------------------------
void rechercheVND(Solution* sol) {

    int k = 1;
    int ameliore;


    // FILE* sortie = fopen("plot/out.res", "w");
    constructionGloutonne(sol);

    // printf("z initial : %d\n", sol->z);

    int nbIter = 1;
    while(k <= 3) {

        ameliore = rechercheLocale(sol, k);

        // fprintf(sortie, "%d %d %d %d\n", nbIter, sol->z, sol->z, sol->z);
        // printf("nbIter : %d, z : %d\n", nbIter, sol->z);

        if(ameliore) {
            k = 1;
            // printf("nouveau z : %d\n", sol->z);
        } else {
            k ++;
        }
        nbIter ++;
    }

    // fclose(sortie);
}


//------------------------------------------------------------------------------
void rechercheVNS(Solution* sol, int option) {

    int k = 1;
    int nbIt = 0, nbItMax = 100;

    // FILE* sortie = fopen("plot/out.res", "w");

    constructionGloutonne(sol);

    // printf("z initial : %d\n", sol->z);

    Solution voisin;
    creerSolution(sol->pb, &voisin);

    // ensemble des solutions élites pour le path relinking
    EliteSet set;
    creerEliteSet(&set);
    ajouterSolution(&set, sol); // ajout de la solution gloutonne

    int relk = 0;

    while(nbIt < nbItMax) {

        k = 1;

        while(k <= 5) {

            // printf("nbIt : %d\n", nbIt);

            // on part de la solution actuelle qui est recopiée
            copierSolution(sol, &voisin);

            // choix d'un voisin aléatoire
            int rea = 0;
            rea = voisinAlea3(&voisin, k);

            int initial = voisin.z;

            // recherche locale sur ce voisin
            if(rea) {
                rechercheLocale(&voisin, k);
            }

            if(rea && voisin.z > sol->z) {
                copierSolution(&voisin, sol);
                k = 1; // une amélioration donc on revient au premier voisinage
                nbItMax = nbIt + 100; // critère d'arrêt : pas d'amélioration depuis une certain nombre d'itération

                printf("z : %d\n", sol->z);

                // ajout des solutions améliorantes dans l'eliteSet
                ajouterSolution(&set, sol);

            } else {

                if(rea) {
                    ajouterSolution(&set, sol);
                }
                k ++;
            }

            /*if(rea) {
                fprintf(sortie, "%d %d %d %d\n", nbIt, initial, voisin.z, sol->z);
            }*/
            nbIt ++;
        }

        // printf("zMax : %d, nbIt : %d\n", sol->z, nbIt);

    }

    // printf("taille de l'élite set : %d\n", set.taille);

    /*Solution solA;
    Solution solB;
    Solution res;
    creerSolution(sol->pb, &solA);
    creerSolution(sol->pb, &solB);
    creerSolution(sol->pb, &res);
    // lancement du path relinking entre des solution de l'elite set
    for(int i = 0; i < set.taille/2; i++) {

        eliteRndm(&set, &solA);
        eliteRndm(&set, &solB);

        path_relinking(&solA, &solB, &res);

        if(res.z > sol->z) {
            printf("le path relinking donne : z = %d\n", res.z);
            copierSolution(&res, sol);
        }

    }*/

    detruireSolution(&voisin);

    detruireEliteSet(&set);

    // fclose(sortie);

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

//--------------------------------------------------------------------------------
void path_relinking(Solution* best , Solution* worst, Solution* nouv) {

    nouv->z = (best->z < worst->z ? best->z : worst->z);

    Solution temp;
    creerSolution(worst->pb, &temp);

    int i = 0;
    while(i < worst->nbVar1) {
        // différence entre best et val, la solution est modifiée
        if(best->valeur[worst->var1[i]] == 0) {
            passerVariable0(worst, i);

            copierSolution(worst, &temp);
            rechercheLocale(&temp, 1);

            if(temp.z > nouv->z) {
                copierSolution(&temp, nouv);
                // printf("Meilleur z : %d\n", nouv->z);
            }
        } else {
            i ++;
        }
    }

    i = 0;
    while(i < worst->nbVar0) {
        // différence entre best et val, la solution est modifiée
        if(best->valeur[worst->var0[i]] == 1) {
            passerVariable1(worst, i);

            copierSolution(worst, &temp);
            reconstruireSolution(&temp);
            if(temp.z >= nouv->z) {

                rechercheLocale(&temp, 1);
                if(temp.z > nouv->z) {
                    copierSolution(&temp, nouv);
                    // printf("Meilleur z : %d\n", nouv->z);
                }
            }
        } else {
            i ++; // <- là ne posez pas trop de question, ça vient de la façon dont le tableau des indices des var à 0 est mis à jours
        }
    }

    detruireSolution(&temp);

}
