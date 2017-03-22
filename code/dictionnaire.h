#ifndef _DICO_H
#define _DICO_H

#include "liste.h"

#define TAILLE_CHAINE 50

// description d'une entree de dictionnaire
typedef struct entree {
    char terme[TAILLE_CHAINE]; // la chaine de caracteres représentant le terme (i.e., mot)
    double idf;                // l'idf associée (valeur temporaire)
    double tf;                 // la frequence du terme dans un document (valeur temporaire)
} ENTREE;                      // ENTREE est le type de cette structure


int compare_cle(void* chaine_cle, void* entree2);
int compare_entree(void* entree1, void* entree2);
void afffiche_entree(void* entree1);
void reset_tf_dico(Liste* dico,int taille_dico);
int hash(unsigned char* chaine, int N);
void affiche_dico(Liste* dico, int taille_table );
void libere_dico(Liste* dico, int taille_table );
ENTREE* get(char* cle, Liste* dico, int N );
char* put(char* chaine, Liste* dico, int taille_table );


#endif
