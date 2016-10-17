#include <stdio.h>
#include <stdlib.h>

#include "Probleme.h"

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    Probleme pb;
    chargerProbleme("test.txt", &pb);

    afficherProbleme(&pb);

    detruireProbleme(&pb);

    return 0;

}
