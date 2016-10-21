/**
 * \file Voisinage.c
 * \brief implémentation des fonctions de calcul de voisinage
 */

#include <stdlib.h>
#include <stdio.h>

#include "Voisinage.h"

//------------------------------------------------------------------------------
int majSommeCtr1(Solution* sol, int ind) {

    int rea = 1;

    // mise à jour des sommes des contraintes
    for(int i = 0; i < sol->pb->nbCtr; i++) {
        if(sol->pb->contrainte[i][ind] == 1) {
            sol->sommeCtr[i] ++;
        }

        if(sol->sommeCtr[i] > 1) {
            rea = 0;
        }
    }

    return rea;
}

//------------------------------------------------------------------------------
int majSommeCtr0(Solution* sol, int ind) {

    int rea = 1;

    // mise à jour des sommes des contraintes
    for(int i = 0; i < sol->pb->nbCtr; i++) {
        if(sol->pb->contrainte[i][ind] == 1) {
            sol->sommeCtr[i] --;
        }

        // peut être vrai à cause d'une modif précédente non vérifiée (pour k/p = 1/2)
        if(sol->sommeCtr[i] > 1) {
            rea = 0;
        }
    }

    return rea;

}

//------------------------------------------------------------------------------
int echange01(Solution* sol) {
    int zMax = -1;
    int zVois = sol->z;

    int ind0Max;

    for(int i = 0; i < sol->nbVar0; i++) { // une variable affectée à 0 est réaffectée à 1

        int rea = majSommeCtr1(sol, sol->var0[i]);
        zVois += sol->pb->cout[sol->var0[i]];

        if(rea == 1 && (zMax == -1 || zVois > zMax)) {
            zMax = zVois;
            ind0Max = i;
        }

        zVois -= sol->pb->cout[sol->var0[i]];
        majSommeCtr0(sol, sol->var0[i]);

    }

    int ameliore = 0;

    if(zMax > sol->z) {
        ameliore = 1;
        sol->z = zMax;
        sol->valeur[sol->var0[ind0Max]] = 1;
        majSommeCtr1(sol, sol->var0[ind0Max]);
        sol->var1[sol->nbVar1] = sol->var0[ind0Max];
        sol->nbVar1 ++;
        sol->nbVar0 --;
        sol->var0[ind0Max] = sol->var0[sol->nbVar0];
    }

    return ameliore;
}

//------------------------------------------------------------------------------
int echange11(Solution* sol) {

    int zMax = -1;
    int zVois = sol->z;

    int ind0Max; // variable à affecter à 1
    int ind1Max; // variable à affecter à 0

    for(int i = 0; i < sol->nbVar0; i++) { // une variable affectée à 0 est réaffectée à 1

        // la variable passe à 1
        // mise à jour des somme des contraintes
        majSommeCtr1(sol, sol->var0[i]);
        zVois += sol->pb->cout[sol->var0[i]];

        for(int j = 0; j < sol->nbVar1; j++) { // une variable affectée à 1 est réaffectée à 0

            // la variable var0[i] passe à 1 et la variables var1[j] passe à 0

            // test de réalisabilité et mise à jour des sommes en même temps
            int rea = majSommeCtr0(sol, sol->var1[j]);
            zVois -= sol->pb->cout[sol->var1[j]];

            // si la solution est réalisable, on peut comparer la valeur du max
            if(rea == 1) {
                if(zMax == -1 || zVois > zMax) {
                    zMax = zVois;
                    ind0Max = i;
                    ind1Max = j;
                }

            }

            // dé mise à jour
            majSommeCtr1(sol, sol->var1[j]);
            zVois += sol->pb->cout[sol->var1[j]];

        }

        // dé mise à jour de la somme des contraintes
        majSommeCtr0(sol, sol->var0[i]);
        zVois -= sol->pb->cout[sol->var0[i]];

    }

    int ameliore = 0;

    if(zMax > sol->z) { // la meilleure solution a été améliorée, elle est mise à jour
        sol->z = zMax;

        sol->valeur[sol->var0[ind0Max]] = 1;
        sol->valeur[sol->var1[ind1Max]] = 0;

        // re mise à jour des sommes des contraintes
        majSommeCtr1(sol, sol->var0[ind0Max]);
        majSommeCtr0(sol, sol->var1[ind1Max]);

        // swap entre les indices modifiés dans le tableaux des indices à 0 et à 1
        echanger(&sol->var0[ind0Max], &sol->var1[ind1Max]);

        // une amélioration de la fonction objectif a été faite
        ameliore = 1;
    }

    return ameliore;
}

//------------------------------------------------------------------------------
int echange12(Solution* sol) {

    int zMax = -1;
    int zVois = sol->z;

    int ind0Max1; // variable à affecter à 1
    int ind0Max2; // deuxième variable à affecter à 1
    int ind1Max; // variable à affecter à 0

    // une variable est passée de 1 à 0
    for(int i = 0; i < sol->nbVar1; i++) {

        zVois -= sol->pb->cout[sol->var1[i]];
        majSommeCtr0(sol, sol->var1[i]);

        // une première variables est passée de 0 à 1
        for(int j = 0; j < sol->nbVar0; j++) {

            zVois += sol->pb->cout[sol->var0[j]];
            majSommeCtr1(sol, sol->var0[j]);

            // une seconde variable est passée de 0 à 1
            for(int k = j+1; k < sol->nbVar0-1; k++) {

                zVois += sol->pb->cout[sol->var0[k]];
                int rea = majSommeCtr1(sol, sol->var0[k]);

                // si la solution est réalisable, mise à jour
                if(rea) {

                    if(zMax == -1 || zVois > zMax) { // amélioration de voisin trouvé
                        zMax = zVois;
                        ind1Max = i;
                        ind0Max1 = j;
                        ind0Max2 = k;
                    }

                }

                zVois -= sol->pb->cout[sol->var0[k]];
                majSommeCtr0(sol, sol->var0[k]);

            }

            zVois -= sol->pb->cout[sol->var0[j]];
            majSommeCtr0(sol, sol->var0[j]);

        }

        zVois += sol->pb->cout[sol->var1[i]];
        majSommeCtr1(sol, sol->var1[i]);


    }

    int ameliore = 0;

    if(zMax > sol->z) {

        ameliore = 1;

        // passage des valeurs à 1 dans la solution
        sol->valeur[sol->var1[ind1Max]] = 0;
        sol->valeur[sol->var0[ind0Max1]] = 1;
        sol->valeur[sol->var0[ind0Max2]] = 1;

        // maj fonction objectif
        sol->z = zMax;

        // maj somme contraintes
        majSommeCtr1(sol, sol->var0[ind0Max1]);
        majSommeCtr1(sol, sol->var0[ind0Max2]);
        majSommeCtr0(sol, sol->var1[ind1Max]);

        // maj des indices
        echanger(&sol->var0[ind0Max1], &sol->var1[ind1Max]);
        sol->var1[sol->nbVar1] = sol->var0[ind0Max2];
        sol->nbVar1 ++;
        sol->nbVar0 --;
        sol->var0[ind0Max2] = sol->var0[sol->nbVar0];


    }

    return ameliore;
}

//------------------------------------------------------------------------------
void echanger(int* val1, int* val2) {
    int tmp = *val1;
    *val1 = *val2;
    *val2 = tmp;
}

//------------------------------------------------------------------------------
int echangeAlea(Solution* sol, int k, int p) {

    // indices des indices des variables à modifier
    int *ind0 = malloc((long unsigned int)p*sizeof(int));
    int *ind1 = malloc((long unsigned int)k*sizeof(int));

    int rea = 1;

    int indice;

    // variables qui passent de 1 à 0
    for(int i = 0; i < k; i++) {

        indice = aleaBorne(0, sol->nbVar1-1);
        ind1[i] = sol->var1[indice];
        majSommeCtr0(sol, ind1[i]);
        sol->z -= sol->pb->cout[ind1[i]];
        sol->nbVar1 --;
        sol->var1[indice] = sol->var1[sol->nbVar1];

        // variables qui passent de 0 à 1
        for(int j = 0; j < p; j++) {

            indice = aleaBorne(0, sol->nbVar0-1);
            ind0[j] = sol->var0[indice];
            rea = majSommeCtr1(sol, ind0[j]);
            sol->z += sol->pb->cout[ind0[j]];
            sol->nbVar0 --;
            sol->var0[indice] = sol->var0[sol->nbVar0];

        }

    }

    if(rea) { // les changements sont gardés et finis

        for(int i = 0; i < k; i++) {
            sol->valeur[ind1[i]] = 0;
            sol->var0[sol->nbVar0] = ind1[i];
            sol->nbVar0 ++;
        }
        for(int i = 0; i < p; i++) {
            sol->valeur[ind0[i]] = 1;
            sol->var1[sol->nbVar1] = ind0[i];
            sol->nbVar1 ++;
        }

    } else { // annulation des changements

        // variables qui étaient passées de 1 à 0
        for(int i = 0; i < k; i++) {
            sol->var1[sol->nbVar1] = ind1[i];
            sol->nbVar1 ++;
            majSommeCtr1(sol, ind1[i]);
            sol->z += sol->pb->cout[ind1[i]];
        }
        for(int i = 0; i < p; i++) {
            sol->var0[sol->nbVar0] = ind0[i];
            sol->nbVar0 ++;
            majSommeCtr0(sol, ind0[i]);
            sol->z -= sol->pb->cout[ind0[i]];
        }

    }

    free(ind0);
    free(ind1);

    return rea;
}

//------------------------------------------------------------------------------
int aleaBorne(int min, int max) {
    return min + rand()%(max-min+1);
}
