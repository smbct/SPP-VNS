#include <stdio.h>
#include <stdlib.h>

#include "Solution.h"
#include "Voisinage.h"

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
    printf("valeur z = %d\n", sol.z);

    // tentative d'amélioration de la solution
    int continuer;
    do {
        continuer = echange12(&sol);
    } while(continuer);

    printf("Solution après recherche locale : \n");
    afficherSolution(&sol);
    printf("valeur z = %d\n", sol.z);

    // destruction de la solution
    detruireSolution(&sol);

    detruireProbleme(&pb);

    return 0;

}
