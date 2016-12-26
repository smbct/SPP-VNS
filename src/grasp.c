#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "grasp.h"
#include "Voisinage.h"
#include "VNS.h"

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
void grasp(Solution* meilleure , int nbIt , double alpha){


    meilleure->z = 0;
    Solution sol;
    creerSolution(meilleure->pb , &sol);

    for(int i = 0 ; i < nbIt ; i++){

        //greedyRandomizedC
        greedyRandomizedC(&sol , alpha);

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
    int nbAlpha = 4;
    double alpha;
    float proba;

    float listeAlpha [4] = {0.30 , 0.45 , 0.65 , 0.85};
    float p[nbAlpha];
    float q[nbAlpha];
    float somq = 0.0;
    float zAvg[nbAlpha];
    float zBest[nbAlpha];
    float zWorst[nbAlpha];

    int pos;

    for(int i = 0 ; i < nbAlpha ; i++){
        zBest[i] = 0.0;
        zWorst[i] = 0.0;
        zAvg[i] = 0.0;
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

    for(int i = 0 ; i < nbIt ; i++){
        pos = rand()%nbAlpha;
        alpha = listeAlpha[pos];
        printf("alpha = %.3lf\n", listeAlpha[pos]);
        //greedyRandomizedC
        greedyRandomizedC(&sol , alpha);

        //rechercheLocale
        rechercheLocale(&sol , 1);
        rechercheLocale(&sol , 2);

        //update
        if(sol.z > zBest[pos]){
            zBest[pos] = sol.z;
            if(sol.z > meilleure->z){
                copierSolution(&sol , meilleure);
                printf("solution z =  %d\n", sol.z );

            }
        }

        if(sol.z < zWorst[pos] || zWorst[pos] == 0.0){
            zWorst[pos] = sol.z;
        }

        zAvg[pos] = zAvg[pos] + sol.z;

        for(int j = 0 ; j < meilleure->pb->nbVar ; j++){
            sol.valeur[j] = 0;
        }

    }

    for (int i = 0 ; i < nbAlpha ; i++){
        if(zAvg[i] > 0){
            q[i] = ((zAvg[i]/nbAlpha) - zWorst[i])/(zBest[i] - zWorst[i]);
        }
        somq = somq + q[i];
    }

    for(int i = 0 ; i < nbAlpha ; i++){
        p[i] = q[i]/somq;
        if(i > 0){
            p[i] = p[i] + p[i-1];
        }
    }

    for (int i = 0 ; i < nbIt ; i++){
        srand((unsigned int)time(NULL));
        proba = (((float)rand()/(float)(RAND_MAX)) * 1.0);
        //choisir proba et l'alpha correspondant

        if(proba < p[0] ){
            alpha = listeAlpha[0];
        }else if (proba>= p[0] && proba < p[1]){
            alpha = listeAlpha[1];
        }else if (proba>= p[1] && proba < p[2]){
            alpha = listeAlpha[2];
        }else if (proba>= p[2] && proba <= p[3]){
            alpha = listeAlpha[3];
        }

        printf("alpha = %.3lf\n",alpha );

        //greedyRandomizedC
        greedyRandomizedC(&sol , alpha);

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
    }

}
