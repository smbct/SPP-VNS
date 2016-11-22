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

    FILE* sortie = fopen("plot/out.res", "w");

    constructionGloutonne(sol);

    // printf("z initial : %d\n", sol->z);

    Solution voisin;
    creerSolution(sol->pb, &voisin);

    Solution reliee;
    creerSolution(sol->pb, &reliee);

    Solution initiale;
    creerSolution(sol->pb, &initiale);
    copierSolution(sol, &initiale);

    Solution meilleureRelinking;
    creerSolution(sol->pb, &meilleureRelinking);

    Solution resRelinking;
    creerSolution(sol->pb, &resRelinking);

    int relk = 0;

    while(nbIt < nbItMax) {

        k = 1;

        while(k <= 5) {

            // printf("nbIt : %d\n", nbIt);

            // on part de la solution actuelle qui est recopiée
            copierSolution(sol, &voisin);

            // choix d'un voisin aléatoire
            int rea = 0;
            /*if(option == 1) {
                rea = voisinAlea1(&voisin, k);
            } else if(option == 2) {
                rea = voisinAlea2(&voisin, k);
            } else {*/
                rea = voisinAlea3(&voisin, k);
            //}
            int initial = voisin.z;

            // recherche locale sur ce voisin
            if(rea) {
                rechercheLocale(&voisin, k);
            }

            if(rea && voisin.z > sol->z) {
                copierSolution(&voisin, sol);
                k = 1;
                nbItMax = nbIt + 100; // critère d'arrêt : pas d'amélioration depuis une certain nombre d'itération
                // printf("z : %d\n", sol->z);

                // on lance un path relinking
                copierSolution(&voisin, &reliee);
                path_relinking(&initiale, &reliee, &resRelinking);

                if(relk == 0 || resRelinking.z > meilleureRelinking.z) {
                    copierSolution(&resRelinking, &meilleureRelinking);
                    copierSolution(&resRelinking, sol);
                    relk = 1;
                }

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

    if(relk && meilleureRelinking.z > sol->z) {
        copierSolution(&meilleureRelinking, sol);
    }

    detruireSolution(&voisin);
    detruireSolution(&reliee);
    detruireSolution(&resRelinking);
    detruireSolution(&meilleureRelinking);
    detruireSolution(&initiale);

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

//--------------------------------------------------------------------------------
void path_relinking(Solution* best , Solution* worst, Solution* nouv) {

    nouv->z = (best->z < worst->z ? best->z : worst->z);

    /*Solution bestPrim ;
    creerSolution(best->pb, &bestPrim);
    Solution worstPrim;
    creerSolution(best->pb, &worstPrim);

    copierSolution(best , &bestPrim);

    for (int ind = 0 ; ind <  best->pb->nbVar ; ind++){
        if(best->valeur[ind] != worst->valeur[ind]){

            if ((*worst).valeur[ind] == 1){
                worstPrim.valeur[ind] = 0;
                majSommeCtr0(&worstPrim , ind);
            }else{
                worstPrim.valeur[ind] = 1;
                majSommeCtr1(&worstPrim , ind);
            }
            if(worstPrim.nbCtrVio > 0){
                reconstruireSolution(&worstPrim);
            }
            copierSolution(worst , &worstPrim);
            rechercheVNS(&worstPrim , 3);
            initialiserZ(&worstPrim);
            initialiserListeIndices(&worstPrim);
            initialiserSommeCtr(&worstPrim);

            if (worstPrim.z > best->z){
                copierSolution(&worstPrim , best);
            }
        }

    }*/

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
                printf("Meilleur z : %d\n", nouv->z);
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
                    printf("Meilleur z : %d\n", nouv->z);
                }
            }
        } else {
            i ++; // <- là ne posez pas trop de question, ça vient de la façon dont le tableau des indices des var à 0 est mis à jours
        }
    }

    detruireSolution(&temp);

}
