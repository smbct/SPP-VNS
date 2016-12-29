#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "grasp.h"
#include "Voisinage.h"
#include "VNS.h"

#define NB_ALPHA 4

//------------------------------------------------------------------------------
void greedyRandomizedC(Solution* sol , double alpha){
    float min , max;
    float limit;
    int* RCL;  // la liste de candidat
    float * tabUtilite = malloc(((long unsigned int)sol->pb->nbVar)*sizeof(float));

    RCL = malloc (((long unsigned int )sol->pb->nbVar)*sizeof(int) );

    for(int i = 0 ; i < sol->pb->nbVar ; i++){
        tabUtilite[i] = (float) sol->pb->utilite[i];
    }
    //initialisation
    initialiserZ(sol);
    initialiserListeIndices(sol);
    initialiserSommeCtr(sol);

    // trier le tableau
    double temp;
    int temp1;
    int b = 1; //boolean pour savoir si on est a la limit
    int tailleRCL = 0; // taille de la RCL
    int nbVarRestant = sol->pb->nbVar;

    int indCourant = 0;
    int k ;


    while(nbVarRestant > 0 ) {

        //recherche du max et du min
        min = -1; max = tabUtilite[0];
        for(int i = 1 ; i < sol->pb->nbVar ; i++){
            if((min > tabUtilite[i] && tabUtilite[i] != -1 ) || min == -1){
                min = tabUtilite[i];
            }
            if(max < tabUtilite[i]){
                max = tabUtilite[i];
            }
        }

        //calcul de limit
        limit = min + alpha * (max - min);

        //retrouver la taille de la RCL
        tailleRCL = 0;
        for(int i = 0 ; i < sol->pb->nbVar ; i++){
            if(tabUtilite[i] >= limit){
                tailleRCL++;
            }
        }

        //remplissage de la RCL
        temp1 = 0;
        for(int i =0 ; i < sol->pb->nbVar  ; i++){
            if(tabUtilite[i] != -1 && temp1 < tailleRCL){
                RCL[temp1] = i;
                //printf("%d  " , RCL[temp1]);
                temp1++;
            }
        }

        //glouton random;

        indCourant = rand()%(tailleRCL);

        // affectation de la variable à 1 si la variable est utilisable
        passerVariable1(sol , RCL[indCourant]);
        nbVarRestant --;
        tabUtilite[RCL[indCourant]] = -1;

        // suppression des variables ne pouvant plus être affectées à 1
        for(int indCtr = 0; indCtr < sol->pb->nbCtr; indCtr ++) {
            if(sol->pb->contrainte[indCtr][RCL[indCourant]] == 1) {
                // parcours des variables apparaissant dans la contrainte
                for(int indVar = 0; indVar < sol->pb->nbVar; indVar ++) {
                    if(sol->pb->contrainte[indCtr][indVar] == 1 && tabUtilite[indVar] != -1) {
                        nbVarRestant --;
                        tabUtilite[indVar] = -1;
                    }
                }
            }
        }
    }

    free(RCL);
    free(tabUtilite);
}

//------------------------------------------------------------------------------
void greedyRandomizedC2(Solution* sol , double alpha){

    double min , max;
    double limit;
    int* RCL;  // la liste de candidat

    RCL = malloc (((long unsigned int )sol->pb->nbVar)*sizeof(int) );

    int* dispos = malloc((long unsigned int)sol->pb->nbVar*sizeof(int)); // variables pouvant encore être affectées à 1
    for(int i = 0; i < sol->pb->nbVar; i++) {
        dispos[i] = 1;
    }

    // remise à 0 de la solution
    resetSolution(sol);

    // construction de la RCL
    int tailleRCL = 0; // taille de la RCL
    int nbVarRestant = sol->nbVar0;

    int indCourant = 0;

    while(nbVarRestant > 0) {

        //recherche du max et du min
        min = -1.; max = -1.;
        for(int i = 1 ; i < sol->nbVar0; i++){

            if(dispos[sol->var0[i]]) {

                if(min > sol->pb->utilite[sol->var0[i]] || min < 0) {
                    min = sol->pb->utilite[sol->var0[i]];
                }
                if(max < sol->pb->utilite[sol->var0[i]] || max < 0){
                    max = sol->pb->utilite[sol->var0[i]];
                }

            }

        }

        //calcul de limit
        limit = min + alpha * (max - min);

        tailleRCL = 0;

        //remplissage de la RCL
        for(int i = 0 ; i < sol->nbVar0; i++) {
            if(dispos[sol->var0[i]]) {
                if(sol->pb->utilite[sol->var0[i]] >= limit) {
                    RCL[tailleRCL] = i;
                    tailleRCL ++;
                }
            }
        }

        //choix random;

        indCourant = rand()%(tailleRCL);

        int indVar0 = RCL[indCourant];

        // cette variable ne peut plus être sélectionnée
        dispos[sol->var0[indVar0]] = 0;
        nbVarRestant --;

        // suppression des variables ne pouvant plus être affectées à 1
        for(int indCtrVar = 0; indCtrVar < sol->pb->nbCtrVar[sol->var0[indVar0]]; indCtrVar ++) {

            int indCtr = sol->pb->ctrVar[sol->var0[indVar0]][indCtrVar];

            // parcours des variables apparaissant dans la contrainte
            for(int indVar = 0; indVar < sol->pb->nbVar; indVar ++) {
                if(sol->pb->contrainte[indCtr][indVar] == 1 && dispos[indVar] == 1) {
                    nbVarRestant --;
                    dispos[indVar] = 0;
                }
            }

        }

        // affectation de la variable à 1 si la variable est utilisable
        passerVariable1(sol , indVar0);

    }

    /*printf("solution greedy randomizée :\n");
    afficherSolution(sol);*/

    free(RCL);
    free(dispos);
}


//------------------------------------------------------------------------------
void grasp(Solution* meilleure , int nbIt , double alpha){

    meilleure->z = 0;
    Solution sol;
    creerSolution(meilleure->pb , &sol);

    for(int i = 0 ; i < nbIt ; i++){

        //greedyRandomizedC
        greedyRandomizedC2(&sol , alpha);

        //rechercheLocale
        rechercheLocale(&sol , 1);
        rechercheLocale(&sol , 2);
        //rechercheVNS(&sol, 3);
        //rechercheVND(&sol);
        //update
        if(sol.z > meilleure->z){
            copierSolution(&sol , meilleure);
            printf("solution z =  %d\n", sol.z );
        }
        for(int j = 0 ; j < meilleure->pb->nbVar ; j++){
            sol.valeur[j] = 0;
        }
        //detruireSolution(&sol);
    }
    detruireSolution(&sol);
}


//------------------------------------------------------------------------------
void reactiveGrasp(Solution * meilleure , int nbIt) {
    const int Nalpha = 50;

    double alpha;
    double proba;

    double listeAlpha [] = {0.30 , 0.45 , 0.65 , 0.85};

    // double listeAlpha [] = {0.05, 0.15 , 0.25 , 0.35 , 0.45, 0.55, 0.65, 0.75, 0.85, 0.95};


    double p[NB_ALPHA];
    double q[NB_ALPHA];
    double somq = 0.0;
    double zSomme[NB_ALPHA];
    int nbChoix[NB_ALPHA]; // nombre de fois que le alpha a été choisi, pour le calcul de la moyenne
    double zBest;
    double zWorst;

    int pos;

    zBest = -1.;
    zWorst = -1.;
    for(int i = 0 ; i < NB_ALPHA ; i++){
        zSomme[i] = 0.0;
        nbChoix[i] = 0;
        p[i] = 1./(double)NB_ALPHA;
        q[i] = 0.;
    }

    meilleure->z = 0;
    Solution solBest;
    Solution solWorst;
    Solution sol;
    creerSolution(meilleure->pb , &sol);
    creerSolution(meilleure->pb , &solBest);
    creerSolution(meilleure->pb , &solWorst);
    solWorst.z = 0;
    solBest.z = 0;

    for(int it = 1 ; it <= nbIt ; it++){

        // tirage du alpha avec la roulette biasée
        proba = (((double)rand()/(double)(RAND_MAX)) * 1.0);
        alpha = 0;
        double somme = 0.;
        pos = 0;
        while(pos < NB_ALPHA && somme < proba) {
            somme += p[pos];
            if(somme < proba) {
                pos ++;
            }
        }

        /*printf("proba : %lf\n", proba);
        printf("somme : %lf\n", somme);*/

        /*printf("nbChoix : ");
        for(int i = 0; i < NB_ALPHA; i++) {
            printf("%d, ", nbChoix[i]);
        }
        printf("\n\n");*/

        /*printf("q : ");
        for(int i = 0; i < NB_ALPHA; i++) {
            printf("%lf, ", q[i]);
        }
        printf("\n\n");

        printf("p : ");
        for(int i = 0; i < NB_ALPHA; i++) {
            printf("%lf, ", p[i]);
        }
        printf("\n\n");

        printf("zSomme : ");
        for(int i = 0; i < NB_ALPHA; i++) {
            printf("%lf, ", zSomme[i]);
        }
        printf("\n\n");*/

        alpha = listeAlpha[pos];
        // printf("alpha = %.3lf\n", listeAlpha[pos]);
        //greedyRandomizedC
        greedyRandomizedC2(&sol , alpha);

        //rechercheLocale
        rechercheLocale(&sol , 1);
        rechercheLocale(&sol , 2);

        //update
        if(sol.z > zBest || zBest < 0.){
            zBest = sol.z;
            if(sol.z > meilleure->z){
                copierSolution(&sol , meilleure);
                // printf("solution z =  %d\n", sol.z );
            }
        }

        if(sol.z < zWorst || zWorst < 0.){
            zWorst = sol.z;
        }

        zSomme[pos] += sol.z;
        nbChoix[pos] ++;

        // mise à jour des probas
        if(it % Nalpha == 0) {

            somq = 0.;
            for (int j = 0 ; j < NB_ALPHA ; j++){
                double zAvg = zSomme[j] / (double)nbChoix[j];
                q[j] = (zAvg - zWorst)/(zBest - zWorst);
                somq += q[j];
            }

            for(int j = 0 ; j < NB_ALPHA ; j++){
                p[j] = q[j]/somq;
            }

        }

    }


}
