#include <stdio.h>
#include <stdlib.h>

#include "Solution.h"
#include "Voisinage.h"
#include "VNS.h"

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    Probleme pb;

    chargerProbleme(/*"test.txt"*/"../instances/pb_100rnd0100.dat", &pb);

    // afficherProbleme(&pb);

    Solution sol;
    creerSolution(&pb, &sol);

    rechercheVND(&sol);

    printf("Solution après VND : \n");
    afficherSolution(&sol);
    printf("valeur z = %d\n", sol.z);

    // destruction de la solution
    detruireSolution(&sol);

    detruireProbleme(&pb);

    return 0;

}
