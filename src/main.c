#include <stdio.h>
#include <stdlib.h>

#include "Solution.h"

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    Probleme pb;

    chargerProbleme("test.txt"/*"../instances/pb_500rnd0100.dat"*/, &pb);

    afficherProbleme(&pb);

    Solution sol;
    creerSolution(&pb, &sol);
    constructionGloutonne(&sol);

    printf("\nSolution actuelle : \n");
    afficherSolution(&sol);

    // amélioration de la solution

    int continuer;
    do {
        continuer = echange12(&sol);
    } while(continuer);

    printf("Solution après recherche locale : \n");
    afficherSolution(&sol);

    initialiserSommeCtr(&sol);

    printf("sommeCtr : \n");
    for(int i = 0; i < pb.nbCtr; i++) {
        printf("%d, ", sol.sommeCtr[i]);
    }
    printf("\n");

    int val = 0;
    for(int i = 0; i < pb.nbVar; i++) {
        if(sol.valeur[i] == 1) {
            val += pb.cout[i];
        }
    }
    printf("valeur z = %d\n", val);

    // destruction de la solution
    detruireSolution(&sol);

    detruireProbleme(&pb);

    return 0;

}
