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

    // chargerProbleme(/*"instances/pb_100rnd0100.dat"*//*"instances/pb_500rnd1500.dat"*//*"test.txt"*/"instances/pb_200rnd0900.dat", &pb);
    // chargerProbleme("instances/pb_100rnd0100.dat"/*"instances/pb_500rnd1500.dat"*/, &pb);
    // chargerProbleme("instances/test.txt", &pb);
    // chargerProbleme(argv[1], &pb);


    chargerProbleme("instances/pb_500rnd1300.dat", &pb);

    // chargerProbleme(argv[1], &pb);

    // afficherProbleme(&pb);

    Solution sol;
    creerSolution(&pb, &sol);
    constructionGloutonne(&sol);
    printf("Après construction gloutonne : %d\n", sol.z);

    rechercheVNS(&sol, 3);

    printf("Solution après VNS : \n");
    afficherSolution(&sol);

    detruireProbleme(&pb);




    return 0;

}
