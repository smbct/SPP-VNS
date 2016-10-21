#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Solution.h"
#include "Voisinage.h"
#include "VNS.h"

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    srand((unsigned int)time(NULL));

    Probleme pb;

    chargerProbleme(/*"test.txt"*/"../instances/pb_100rnd0100.dat", &pb);

    // afficherProbleme(&pb);

    Solution sol;
    creerSolution(&pb, &sol);

    constructionGloutonne(&sol);

    printf("Solution gloutonne : \n");
    afficherSolution(&sol);

    int rea = 0;
    Solution cpy;
    creerSolution(&pb, &cpy);
    copierSolution(&sol, &cpy);
    int nbIt = 0;
    do {
        rea = echangeAlea(&cpy, 1, 1);
        nbIt ++;
    } while(!rea);
    printf("nbIt : %d\n", nbIt);
    afficherSolution(&cpy);
    printf("valeur z = %d\n", cpy.z);

    /*rechercheVND(&sol);

    printf("Solution après VND : \n");
    afficherSolution(&sol);
    printf("valeur z = %d\n", sol.z);*/

    // destruction de la solution
    detruireSolution(&sol);

    detruireProbleme(&pb);

    return 0;

}
