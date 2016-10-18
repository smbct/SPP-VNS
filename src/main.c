#include <stdio.h>
#include <stdlib.h>

#include "Solution.h"


//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    Probleme pb;

    chargerProbleme("test.txt", &pb);

    afficherProbleme(&pb);

    Solution sol;
    creerSolution(&pb, &sol);
    constructionGloutonne(&sol);

    printf("\nSolution actuelle : \n");
    afficherSolution(&sol);

    // destruction de la solution
    detruireSolution(&sol);

    detruireProbleme(&pb);

    return 0;

}
