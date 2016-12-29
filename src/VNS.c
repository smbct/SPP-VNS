/**
 * \file VNS.c
 * \brief implémentation de fonctions pour la méthaheuristique VNS et VND
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "VNS.h"
#include "Voisinage.h"
#include "EliteSet.h"

#include "ListeRecherche.h"

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

                // printf("z : %d\n", sol->z);

                // ajout des solutions améliorantes dans l'eliteSet
                ajouterSolution(&set, sol);

            } else {

                if(set.taille < 30) {
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

    printf("Avec VNS, on obtient : z = %d\n", sol->z);

    /*printf("lancement du path relinking\n");
    // printf("taille de l'élite set : %d\n", set.taille);

    Solution solA;
    Solution solB;
    Solution res;
    creerSolution(sol->pb, &solA);
    creerSolution(sol->pb, &solB);
    creerSolution(sol->pb, &res);

    // lancement du path relinking entre des solution de l'elite set
    for(int i = 0; i < set.taille/2; i++) {

        eliteRndm(&set, &solA);
        eliteRndm(&set, &solB);

        path_relinking_2(&solA, &solB, &res);

        if(res.z > sol->z) {
            // printf("le path améliore : z = %d\n", res.z);

            afficherSolution(&res);

            copierSolution(&res, sol);
        }

    }*/

    detruireSolution(&voisin);

    detruireEliteSet(&set);

    // fclose(sortie);

}

//------------------------------------------------------------------------------
void rechercheVNS2(Solution* sol) {

    // variante avec la liste de recherche

    int k = 1;
    int nbIt = 0;

    constructionGloutonne(sol);
    printf("Après construction gloutonne : %d\n", sol->z);

    Solution voisin;
    creerSolution(sol->pb, &voisin);

    ListeRecherche liste;
    initialiserListeRecherche(&liste);
    ajouterSolutionListeRecherche(&liste, sol);

    int nbItMax[30];
    nbItMax[0] = 10;
    nbItMax[1] = 5;
    nbItMax[2] = 3;
    for(int i = 3; i < 30; i++) {
        nbItMax[i] = 1;
    }


    int continuer;

    int nbNoeud = 0;

    while(liste.taille > 0) {

        k = 1;
        continuer = 1;



        /*if(liste.dernier->nbIt >= nbItMax[liste.taille-1]) { // le temps accordé à ce noeud est épuisé, il est retiré
            retirerSolutionListeRecherche(&liste);
            continuer = 0;
        }*/

        while(continuer && k <= 5) {

            printf("taille de la liste : %d\n", liste.taille);
            printf("nbIt dernier maillon : %d\n", liste.dernier->nbIt);
            printf("nbItMax actuel : %d\n", nbItMax[liste.taille-1]);
            printf("meilleur z : %d\n\n", sol->z);

            // on part de la solution actuelle qui est recopiée
            copierSolution(&liste.dernier->sol, &voisin);

            printf("a\n");

            // choix d'un voisin aléatoire
            int rea = 0;
            rea = voisinAlea3(&voisin, k);

            printf("b\n");

            int initial = voisin.z;

            // recherche locale sur ce voisin
            if(rea) {
                printf("début local search : %d\n", k);
                rechercheLocale(&voisin, k);
                printf("fin local search\n");
            }

            printf("c\n");

            // si le voisin trouvé est meilleur que la solution actuelle connue, on bascule vers ce voisin là
            if(rea && voisin.z > liste.dernier->sol.z) {

                // branchement dans la liste de recherche
                ajouterSolutionListeRecherche(&liste, &voisin);
                nbNoeud ++;

                continuer = 0;

                // mise à jour de la meilleure solution connue si nécessaire
                if(voisin.z > sol->z) {
                    copierSolution(&voisin, sol);
                    printf("z : %d\n", sol->z);
                }

            } else {
                k ++;
            }

            printf("d\n");

            if(liste.dernier->nbIt >= nbItMax[liste.taille-1]) { // le temps accordé à ce noeud est épuisé, il est retiré
                retirerSolutionListeRecherche(&liste);
                continuer = 0;
            } else {
                liste.dernier->nbIt ++;
            }

        }

    }

    printf("nbNoeud : %d\n", nbNoeud);

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

    // les variables à 1 sont passées à 0
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

    // les variables à 0 sont passées à 1
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


//------------------------------------------------------------------------------
void path_relinking_2(Solution* solA, Solution* solB, Solution* nouv) {

    // reconstruction pas ok -> solution non admissible -> à voir

    // printf("lancement du nouveau path relinking : \n");

    int bestZ = solA->z > solB->z ? solA->z : solB->z;
    copierSolution(solA->z > solB->z ? solA : solB, nouv);

    // sens : on se dirige de solA vers solB

    Solution temp;
    creerSolution(solA->pb, &temp);

    int nbVar0 = solA->nbVar0;
    int nbVar1 = solA->nbVar1;
    int cpt0 = 0;
    int cpt1 = 0;
    int sens = 0;
    int changement = 0;
    // stratégie : on alterne les variables qui passent à 1 et celles qui passent à 0

    while(cpt0 < nbVar0 && cpt1 < nbVar1) {

        changement = 0;

        if(sens == 0) {

            if(solB->valeur[solA->var0[cpt0]] == 1) { // la variable doit passer à 1
                changement = 1;
                passerVariable1(solA, cpt0);
                nbVar0 --;
            } else {
                cpt0 ++;
            }

        } else {

            if(solB->valeur[solA->var1[cpt1]] == 0) {
                changement = 1;
                passerVariable0(solA, cpt1);
                nbVar1 --;
            } else {
                cpt1 ++;
            }

        }

        if(changement) {

            copierSolution(solA, &temp);

            if(temp.nbCtrVio > 0) {
                reconstruireSolution(&temp);
            }

            rechercheLocale(&temp, 1);
            rechercheLocale(&temp, 2);
            rechercheLocale(&temp, 3);

            if(temp.z > bestZ) {
                bestZ = temp.z;
                copierSolution(&temp, nouv);
            }

            sens = 1-sens;
        }

    }

}
