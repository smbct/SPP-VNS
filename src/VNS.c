/**
 * \file VNS.c
 * \brief implémentation de fonctions pour la méthaheuristique VNS et VND
 */

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
int rechercheLocale(Solution* sol, int k) {

    int amelioration = 0;
    int ameliore = 0;

    do {

        switch(k) {
            case 1:
                ameliore = echange01(sol);
                break;
            case 2:
                ameliore = echange01(sol);
                ameliore = (ameliore || echange11(sol));
                break;
            case 3:
                ameliore = echange01(sol);
                ameliore = (ameliore || echange11(sol));
                ameliore = (ameliore || echange12(sol));
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
