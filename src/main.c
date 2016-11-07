#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Solution.h"
#include "Voisinage.h"
#include "VNS.h"

void resoudre(Probleme* pb, int option) {

    Solution sol;
    creerSolution(pb, &sol);

    if(option == 1) {
        rechercheVND(&sol);
    } else if(option == 2) {
        rechercheVNS(&sol, 1);
    } else if(option == 3) {
        rechercheVNS(&sol, 2);
    } else {
        rechercheVNS(&sol, 3);
    }

    printf("%d\n", sol.z);

    // destruction de la solution
    detruireSolution(&sol);
}

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    if(argc > 1) {

        srand((unsigned int)time(NULL));

        Probleme pb;

        /* doute sur pb_200rnd0500.dat -> glpk trouve 183*/

        // chargerProbleme(/*"instances/pb_500rnd1500.dat"*//*"test.txt"*//*"instances/pb_200rnd0900.dat"*/, &pb);
        chargerProbleme(argv[1], &pb);

        // afficherProbleme(&pb);

        resoudre(&pb, 3);

        // printf("Solution après VNS : \n");
        // afficherSolution(&sol);

        detruireProbleme(&pb);

    } else {
        printf("Erreur, pas d'instance en entrée\n");
    }


    return 0;

}
