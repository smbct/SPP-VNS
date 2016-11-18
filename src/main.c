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

    chargerProbleme("instances/pb_100rnd0100.dat"/*"instances/pb_500rnd1500.dat"*//*"test.txt"*//*"instances/pb_200rnd0900.dat"*/, &pb);
    // chargerProbleme(argv[1], &pb);

    // afficherProbleme(&pb);

    Solution sol;
    creerSolution(&pb, &sol);

    rechercheVNS(&sol, 3);

    printf("Solution après VNS : \n");
    afficherSolution(&sol);


    Solution solA, solB;
    creerSolution(&pb, &solA);
    creerSolution(&pb, &solB);
    path_relinking(&solA, &solB);

    printf("Meileure trouvée : %d\n", solA.z);

    detruireProbleme(&pb);



    return 0;

}
