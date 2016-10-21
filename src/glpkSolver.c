/**
 * \file glpkSolver.c
 * \brief implémentation d'un solver basé sur glpk pour résoudre le set packing
 */

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

#include "Probleme.h"
#include "Solution.h"

/**
 * \brief résolution du problème du set packing avec glpk
 * \param sol une solution du problème
 */
void resoudre(Solution* sol);

//------------------------------------------------------------------------------
int main(int argc, char** argv) {

    if(argc > 1) {

        Probleme pb;

        chargerProbleme(argv[1], &pb);

        // afficherProbleme(&pb);

        Solution sol;
        creerSolution(&pb, &sol);

        resoudre(&sol);

        printf("\nSolution optimale du problème avec glpk : \n");
        afficherSolution(&sol);

        detruireSolution(&sol);
        detruireProbleme(&pb);

    } else {
        printf("Veuillez donner le nom d'une instance valide avec le programme.\n");
    }

    return 0;
}

//------------------------------------------------------------------------------
void resoudre(Solution* sol) {

    Probleme* pb = sol->pb;

    //désactivation du log de glpk
    // glp_term_out(0);

    glp_prob* prob;
    prob = glp_create_prob();
    glp_set_prob_name(prob, "set packing");
    glp_set_obj_dir(prob, GLP_MAX);

    // ajout des variables
    glp_add_cols(prob, pb->nbVar);
    for(int i = 0; i < pb->nbVar; i++) {
        glp_set_col_kind(prob, i+1, GLP_BV); // type de variable
        glp_set_col_bnds(prob, i+1, GLP_DB, 0.0, 1.0); // bornes sur les variables
        glp_set_obj_coef(prob, i+1, (double)pb->cout[i]);
    }

    // ajout des contraintes
    glp_add_rows(prob, pb->nbCtr);
    // bornes des contraintes
    for(int i = 0; i < pb->nbCtr; i++) {
        glp_set_row_bnds(prob, i+1, GLP_UP, 0.0, 1.0); // contrainte du problème
    }

    // calcule du nombre d'élément de la matrice creuse
    int nbCreux = 0;
    for(int indCtr = 0; indCtr < pb->nbCtr; indCtr ++) {
        for(int indVar = 0; indVar < pb->nbVar; indVar ++) {
            if(pb->contrainte[indCtr][indVar] == 1) {
                nbCreux ++;
            }
        }
    }

    printf("nbCreux : %d\n", nbCreux);


    // matrice creuse des contraintes
    int* ia = malloc((long unsigned int)(nbCreux+1)*sizeof(int)); // indices des lignes
    int* ja = malloc((long unsigned int)(nbCreux+1)*sizeof(int)); // indices des colonnes
    double* ar = malloc((long unsigned int)(nbCreux+1)*sizeof(double));

    // initialisation de la matrice des contraintes, f***ing glpk -> indices à 1
    int indCreux = 1;
    for(int indCtr = 0; indCtr < pb->nbCtr; indCtr ++) {
        for(int indVar = 0; indVar < pb->nbVar; indVar ++) {

            if(pb->contrainte[indCtr][indVar] == 1) {
                ia[indCreux] = indCtr+1;
                ja[indCreux] = indVar+1;
                ar[indCreux] = 1.0; // tous les coefficients à 1 si non nuls
                indCreux ++;
            }

        }
    }

    // chargement de la matrice
    glp_load_matrix(prob, nbCreux, ia, ja, ar);

    // résolution
    glp_simplex(prob, NULL);
    glp_intopt(prob, NULL);

    sol->z = (int)glp_mip_obj_val(prob);
    for(int i = 0; i < pb->nbVar; i++) {
        sol->valeur[i] = (int)(glp_mip_col_val(prob, i+1)+0.5);
    }

    free(ia);
    free(ja);
    free(ar);

    glp_delete_prob(prob);

}
