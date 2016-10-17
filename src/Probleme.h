#ifndef PROBLEME_H
#define PROBLEME_H


// structure de donnée contenant une instance du problème
typedef struct {
    int nbVar; // nombre de variable
    int nbCtr; // nombre de contrainte
    int* couts; // vecteur de coûts
    int** contraintes; // matrice des contraintes
} Probleme;

void chargerProbleme(char* nom, Probleme *probleme);
void detruireProbleme(Probleme* probleme);
void afficherProbleme(Probleme* probleme);

#endif // PROBLEME_H
