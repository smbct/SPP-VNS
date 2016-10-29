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

    /* doute sur pb_200rnd0500.dat -> glpk trouve 183*/

    chargerProbleme(/*"test.txt"*/"../instances/pb_200rnd0900.dat", &pb);

    // afficherProbleme(&pb);

    Solution sol;
    creerSolution(&pb, &sol);

    /*constructionGloutonne(&sol);
    printf("Solution gloutonne : \n");
    afficherSolution(&sol);*/

    /*int rea = 0;
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
    printf("valeur z = %d\n", cpy.z);*/

    rechercheVND(&sol);

    printf("Solution après VNS : \n");
    afficherSolution(&sol);
    printf("valeur z = %d\n", sol.z);

    // destruction de la solution
    detruireSolution(&sol);

    detruireProbleme(&pb);

    return 0;

}
