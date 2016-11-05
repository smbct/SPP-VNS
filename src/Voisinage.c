/**
 * \file Voisinage.c
 * \brief implémentation des fonctions de calcul de voisinage
 */

#include <stdlib.h>
#include <stdio.h>

#include "Voisinage.h"

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
int echange01(Solution* sol) {

    int zMax = -1;
    int zVois = sol->z;

    int ind0Max = -1;

    for(int i = 0; i < sol->nbVar0; i++) { // une variable affectée à 0 est réaffectée à 1

        majSommeCtr1(sol, sol->var0[i]);
        zVois += sol->pb->cout[sol->var0[i]];

        if(sol->nbCtrVio == 0) {
            if(zMax == -1 || zVois > zMax) {
                zMax = zVois;
                ind0Max = i;
            }

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
        //
        // printf("z = %d\n", sol->z);
        // printf("p : %d\n\n", ind0Max);

    }

    return ameliore;
}

//------------------------------------------------------------------------------
int echange01Alea(Solution* sol) {

    // chainage pour enregistrer les voisins réalisables (si on en choisit un au hasard)
    typedef struct {
        int ind0;
        struct EchangeRea* suiv;
        int z;
    } EchangeRea;

    EchangeRea* tete = NULL;
    int nbRea = 0;

    int zVois = sol->z;

    for(int i = 0; i < sol->nbVar0; i++) { // une variable affectée à 0 est réaffectée à 1

        majSommeCtr1(sol, sol->var0[i]);
        zVois += sol->pb->cout[sol->var0[i]];

        if(sol->nbCtrVio == 0) {
            nbRea ++;
            EchangeRea* echange = malloc(sizeof(EchangeRea));
            echange->ind0 = i;
            echange->z = zVois;
            echange->suiv = tete;
            tete = echange;
        }

        zVois -= sol->pb->cout[sol->var0[i]];
        majSommeCtr0(sol, sol->var0[i]);

    }

    int realisable = (nbRea > 0);

    printf("nbRea 01 : %d\n", nbRea);

    if(realisable) {
        // choix d'une solution aléatoire
        int tirage = aleaBorne(1, nbRea);
        EchangeRea* maillon = tete;
        for(int i = 1; i < tirage; i++) {
            maillon = maillon->suiv;
        }

        sol->z = maillon->z;
        sol->valeur[sol->var0[maillon->ind0]] = 1;
        majSommeCtr1(sol, sol->var0[maillon->ind0]);
        sol->var1[sol->nbVar1] = sol->var0[maillon->ind0];
        sol->nbVar1 ++;
        sol->nbVar0 --;
        sol->var0[maillon->ind0] = sol->var0[sol->nbVar0];
    }

    // libération de la mémoire
    while(tete != NULL) {
        EchangeRea*  aSup = tete;
        tete = tete->suiv;
        free(aSup);
    }


    return realisable;

}

//------------------------------------------------------------------------------
int echange11(Solution* sol) {

    int zMax = -1;
    int zVois = sol->z;

    int ind0Max = -1; // variable à affecter à 1
    int ind1Max = -1; // variable à affecter à 0

    for(int i = 0; i < sol->nbVar0; i++) { // une variable affectée à 0 est réaffectée à 1

        // la variable passe à 1
        // mise à jour des somme des contraintes
        majSommeCtr1(sol, sol->var0[i]);
        zVois += sol->pb->cout[sol->var0[i]];

        for(int j = 0; j < sol->nbVar1; j++) { // une variable affectée à 1 est réaffectée à 0

            // la variable var0[i] passe à 1 et la variables var1[j] passe à 0

            // test de réalisabilité et mise à jour des sommes en même temps
            majSommeCtr0(sol, sol->var1[j]);
            zVois -= sol->pb->cout[sol->var1[j]];

            // si la solution est réalisable, on peut comparer la valeur du max
            if(sol->nbCtrVio == 0) {
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

        // printf("z = %d\n", sol->z);
        // printf("k : %d\n", ind1Max);
        // printf("p : %d\n\n", ind0Max);
    }

    return ameliore;
}

//------------------------------------------------------------------------------
int echange11Alea(Solution* sol) {

    typedef struct {
        int ind0; // indice de variable à affecter à 1
        int ind1; // indice de variable à affecter à 0
        int z;
        struct EchangeRea* suiv;
    } EchangeRea;

    EchangeRea* tete = NULL;
    int nbRea = 0;

    int zVois = sol->z;

    for(int i = 0; i < sol->nbVar0; i++) { // une variable affectée à 0 est réaffectée à 1

        // la variable passe à 1
        // mise à jour des somme des contraintes
        majSommeCtr1(sol, sol->var0[i]);
        zVois += sol->pb->cout[sol->var0[i]];

        for(int j = 0; j < sol->nbVar1; j++) { // une variable affectée à 1 est réaffectée à 0

            // la variable var0[i] passe à 1 et la variables var1[j] passe à 0

            // test de réalisabilité et mise à jour des sommes en même temps
            majSommeCtr0(sol, sol->var1[j]);
            zVois -= sol->pb->cout[sol->var1[j]];

            // si la solution est réalisable, on peut comparer la valeur du max
            if(sol->nbCtrVio == 0) {
                nbRea ++;
                EchangeRea* maillon = malloc(sizeof(EchangeRea));
                maillon->z = zVois;
                maillon->ind0 = i;
                maillon->ind1 = j;
                maillon->suiv = tete;
                tete = maillon;
            }

            // dé mise à jour
            majSommeCtr1(sol, sol->var1[j]);
            zVois += sol->pb->cout[sol->var1[j]];

        }

        // dé mise à jour de la somme des contraintes
        majSommeCtr0(sol, sol->var0[i]);
        zVois -= sol->pb->cout[sol->var0[i]];

    }

    int realisable = (nbRea > 0);

    printf("nbRea 11 : %d\n", nbRea);

    if(realisable) {

        int tirage = aleaBorne(1, nbRea);
        EchangeRea* choix = tete;
        for(int i = 1; i < tirage; i++) {
            choix = choix->suiv;
        }

        // mise à jour de la solution
        sol->z = choix->z;
        sol->valeur[sol->var0[choix->ind0]] = 1;
        sol->valeur[sol->var1[choix->ind1]] = 0;

        majSommeCtr1(sol, sol->var0[choix->ind0]);
        majSommeCtr0(sol, sol->var1[choix->ind1]);

        echanger(&sol->var0[choix->ind0], &sol->var1[choix->ind1]);

        while(tete != NULL) {
            EchangeRea* aSup = tete;
            tete = tete->suiv;
            free(aSup);
        }
    }

    return realisable;

}

//------------------------------------------------------------------------------
int echange12(Solution* sol) {

    int zMax = -1;
    int zVois = sol->z;

    int ind0Max1 = -1; // variable à affecter à 1
    int ind0Max2 = -1; // deuxième variable à affecter à 1
    int ind1Max = -1; // variable à affecter à 0

    // une variable est passée de 1 à 0
    for(int i = 0; i < sol->nbVar1; i++) {

        zVois -= sol->pb->cout[sol->var1[i]];
        majSommeCtr0(sol, sol->var1[i]);

        // une première variables est passée de 0 à 1
        for(int j = 0; j < sol->nbVar0-1; j++) {

            zVois += sol->pb->cout[sol->var0[j]];
            majSommeCtr1(sol, sol->var0[j]);

            // une seconde variable est passée de 0 à 1
            for(int k = j+1; k < sol->nbVar0; k++) {

                zVois += sol->pb->cout[sol->var0[k]];
                int rea = 0;
                majSommeCtr1(sol, sol->var0[k]);

                // si la solution est réalisable, mise à jour
                if(sol->nbCtrVio == 0) {
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

        // printf("z = %d\n", sol->z);
        // printf("k : %d\n", ind1Max);
        // printf("p : %d, %d\n\n", ind0Max1, ind0Max2);

    }

    return ameliore;
}

//------------------------------------------------------------------------------
int echange12Alea(Solution* sol) {

    typedef struct {
        int z;
        int ind01;
        int ind02;
        int ind1;
        struct EchangeRea* suiv;
    } EchangeRea;


    int zVois = sol->z;

    EchangeRea* tete = NULL;
    int nbRea = 0;

    // une variable est passée de 1 à 0
    for(int i = 0; i < sol->nbVar1; i++) {

        zVois -= sol->pb->cout[sol->var1[i]];
        majSommeCtr0(sol, sol->var1[i]);

        // une première variables est passée de 0 à 1
        for(int j = 0; j < sol->nbVar0-1; j++) {

            zVois += sol->pb->cout[sol->var0[j]];
            majSommeCtr1(sol, sol->var0[j]);

            // une seconde variable est passée de 0 à 1
            for(int k = j+1; k < sol->nbVar0; k++) {

                zVois += sol->pb->cout[sol->var0[k]];
                majSommeCtr1(sol, sol->var0[k]);

                // si la solution est réalisable, mise à jour
                if(sol->nbCtrVio == 0) {
                    nbRea ++;
                    EchangeRea* nouv = malloc(sizeof(EchangeRea));
                    nouv->z = zVois;
                    nouv->ind1 = i;
                    nouv->ind01 = j;
                    nouv->ind02 = k;
                    nouv->suiv = tete;
                    tete = nouv;
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

    int realisable = (nbRea > 0);

    printf("nbRea 1/2 : %d\n", nbRea);

    if(realisable) {

        int tirage = aleaBorne(1, nbRea);
        EchangeRea* choix = tete;
        for(int i = 1; i < tirage; i++) {
            choix = choix->suiv;
        }

        sol->valeur[sol->var1[choix->ind1]] = 0;
        sol->valeur[sol->var0[choix->ind01]] = 1;
        sol->valeur[sol->var0[choix->ind02]] = 1;

        sol->z = choix->z;

        majSommeCtr1(sol, sol->var0[choix->ind01]);
        majSommeCtr1(sol, sol->var0[choix->ind02]);
        majSommeCtr0(sol, sol->var1[choix->ind1]);

        echanger(&sol->var0[choix->ind01], &sol->var1[choix->ind1]);
        sol->var1[sol->nbVar1] = sol->var0[choix->ind02];
        sol->nbVar1 ++;
        sol->nbVar0 --;
        sol->var0[choix->ind02] = sol->var0[sol->nbVar0];
    }

    while(tete != NULL) {
        EchangeRea* aSup = tete;
        tete = tete->suiv;
        free(aSup);
    }

    return realisable;
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

    int rea = 0;

    int indice;

    // variables qui passent de 1 à 0
    for(int i = 0; i < k; i++) {

        indice = aleaBorne(0, sol->nbVar1-1);
        ind1[i] = sol->var1[indice];
        majSommeCtr0(sol, ind1[i]);
        sol->z -= sol->pb->cout[ind1[i]];
        sol->nbVar1 --;
        sol->var1[indice] = sol->var1[sol->nbVar1];
    }

    // variables qui passent de 0 à 1
    for(int i = 0; i < p; i++) {
        indice = aleaBorne(0, sol->nbVar0-1);
        ind0[i] = sol->var0[indice];
        majSommeCtr1(sol, ind0[i]);
        sol->z += sol->pb->cout[ind0[i]];
        sol->nbVar0 --;
        sol->var0[indice] = sol->var0[sol->nbVar0];
    }

    if(sol->nbCtrVio == 0) { // les changements sont gardés et finis
        rea = 1;
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
int voisinAlea(Solution* sol, int k) {

    int realisable = 0; // certains voisinages sont vides pour certaines solutions

    if(k == 1) {
        realisable = echange01Alea(sol);
    } else if(k == 2) {
        realisable = echange11Alea(sol);
    } else {
        realisable = echange12Alea(sol);
    }

    return realisable;
}

//------------------------------------------------------------------------------
int aleaBorne(int min, int max) {
    return min + rand()%(max-min+1);
}

//------------------------------------------------------------------------------
int kpGenerique(int k, int p, Solution* sol) {

    // printf("kp : %d, %d\n", k, p);

    int ameliore = 0;

    kpEchange actuel, meilleur;
    actuel.k = 0;
    actuel.p = 0;
    actuel.z = sol->z;
    actuel.vark = malloc((long unsigned int)k*sizeof(int));
    actuel.varp = malloc((long unsigned int)p*sizeof(int));

    meilleur.z = -1;
    meilleur.k = 0;
    meilleur.p = 0;
    meilleur.vark = malloc((long unsigned int)k*sizeof(int));
    meilleur.varp = malloc((long unsigned int)p*sizeof(int));

    kpGeneriqueRec(k, p, &actuel, sol, &meilleur);

    // application des résultats si une amélioration a été faite
    if(meilleur.z > sol->z) {
        ameliore = 1;
        sol->z = meilleur.z;
        for(int i = 0; i < k; i++) {
            sol->valeur[sol->var1[meilleur.vark[i]]] = 0;
            majSommeCtr0(sol, sol->var1[meilleur.vark[i]]);
            // réajustement des tableaux des indices de variables à 1 et à 0
            sol->var0[sol->nbVar0] = sol->var1[meilleur.vark[i]];
            sol->nbVar0 ++;
            sol->nbVar1 --;
            sol->var1[meilleur.vark[i]] = sol->var1[sol->nbVar1];
        }
        for(int i = 0; i < p; i++) {
            sol->valeur[sol->var0[meilleur.varp[i]]] = 1;
            majSommeCtr1(sol, sol->var1[meilleur.varp[i]]);
            // réajustement des tableaux des indices de variables à 1 et à 0
            sol->var1[sol->nbVar1] = sol->var0[meilleur.varp[i]];
            sol->nbVar1 ++;
            sol->nbVar0 --;
            sol->var0[meilleur.varp[i]] = sol->var0[sol->nbVar0];
        }

        // affichage des la sol retenue
        printf("z = %d\n", sol->z);
        printf("k : ");
        for(int i = 0; i < k; i++) {
            printf("%d, ", meilleur.vark[i]);
        }
        printf("\np : ");
        for(int i = 0; i < p; i++) {
            printf("%d, ", meilleur.varp[i]);
        }
        printf("\n\n");

    }

    free(actuel.vark);
    free(actuel.varp);
    free(meilleur.vark);
    free(meilleur.varp);

    return ameliore;
}

//------------------------------------------------------------------------------
void kpGeneriqueRec(int k, int p, kpEchange* actuel, Solution* sol, kpEchange* meilleur) {

    if(actuel->k < k) { // réaffectation d'une variable affectée à 1 à 0
        actuel->k ++;
        int deb = (actuel->k == 0 ? 0 : actuel->vark[actuel->k-2]+1);
        int fin = sol->nbVar1-k+actuel->k;
        for(int i = deb; i < fin; i++) {
            actuel->vark[actuel->k-1] = i;
            actuel->z -= sol->pb->cout[sol->var1[i]];
            majSommeCtr0(sol, sol->var1[i]);
            kpGeneriqueRec(k, p, actuel, sol, meilleur);
            majSommeCtr1(sol, sol->var1[i]);
            actuel->z += sol->pb->cout[sol->var1[i]];
        }
        actuel->k --;
    } else if(actuel->p < p) { // réaffectation d'une variable affectée à 0 à 1
        actuel->p ++;
        int deb = (actuel->p == 0 ? 0 : actuel->varp[actuel->p-2]+1);
        int fin = sol->nbVar0-p+actuel->p;
        for(int i = deb; i < fin; i++) {
            actuel->varp[actuel->p-1] = i;
            actuel->z += sol->pb->cout[sol->var0[i]];
            majSommeCtr1(sol, sol->var0[i]);
            kpGeneriqueRec(k, p, actuel, sol, meilleur);
            majSommeCtr0(sol, sol->var0[i]);
            actuel->z -= sol->pb->cout[sol->var0[i]];
        }
        actuel->p --;
    } else { // tous les changements ont été fait, on peut tester la réalisibilité de la solution

        // si cet échange est le meilleur trouvé, la solution est mise à jour
        if(sol->nbCtrVio == 0 && (meilleur->z == -1 || actuel->z > meilleur->z)) {
            meilleur->z = actuel->z;
            for(int i = 0; i < actuel->k; i++) {
                meilleur->vark[i] = actuel->vark[i];
            }
            for(int i = 0; i < actuel->p; i++) {
                meilleur->varp[i] = actuel->varp[i];
            }
        }

    }

}
