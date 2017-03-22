
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "liste.h"
#include "dictionnaire.h"



// comparaison entre une chaîne de caractères et une entrée du dictionnaire
int compare_cle(void* chaine_cle, void* entree2) {
    if (chaine_cle && entree2) return strcmp(chaine_cle,((ENTREE*)entree2)->terme);
    return 1;

}

// comparaison entre deux entrées du dictionnaire
int compare_entree(void* entree1, void* entree2) {

    if (entree1 && entree2) return strcmp(((ENTREE*)entree1)->terme,((ENTREE*)entree2)->terme);
    return 1;
}


void afffiche_entree(void* entree1) {
    ENTREE *e = (ENTREE *)entree1;
    printf("'%s' tf=%lf idf=%lf ", e->terme, e->tf, e->idf);

}


// retourne l'index de la chaîne dans la table de hachage
int hash(unsigned char* chaine, int N) {
    unsigned long long a= 31415, b=27183;
    unsigned int i;
    unsigned int hash=chaine[0]%(N-1);
    for (i=1; i<strlen(chaine); i++) {
        a = a*b;
        hash += (a*chaine[i])%(N-1);
    }
    return hash%N;
}


//retourne l'entrée du dictionnaire qui correspond à la clé ou NULL si la clé n'est pas trouvée
ENTREE* get(char* cle, Liste* dico, int N ) {
    int indice = hash(cle, N);
    Liste l = recherche(dico[indice], cle, compare_cle);

    if (l) return (ENTREE*) l-> val;
    return NULL;
}


//ajoute une entrée au dictionnaire si elle ne s'y trouve pas déjà. retourne la chaîne nouvellement ajoutée
char* put(char* cle, Liste* dico, int N ) {
    int indice = hash(cle, N);
    ENTREE* e = get(cle, dico, N);
    // cas ajout
    if (e==NULL) {
        e = calloc(1,sizeof(*e));
        strcpy(e->terme,cle);
        dico[indice] = ajout_tete(e,dico[indice]);
    }
    return e->terme;
}

void reset_tf_dico(Liste* dico,int taille_dico) {
    int i=0;
    Liste p,l;
    for (i=0; i<taille_dico; i++) {
        for ( p = dico[i]; p != NULL; p= p->next) {
            ((ENTREE *) p->val)-> tf = 0.0;
        }
    }
}

void affiche_dico(Liste* dico, int taille_table ) {
    int i=0;
    for (i=0; i<taille_table; i++) {
        visualiser(dico[i], afffiche_entree);
        puts("");
    }

}
void libere_dico(Liste* dico, int taille_table ) {
    int i=0;
    Liste p,l;
    for (i=0; i<taille_table; i++) {
        // libération au cas par cas
        p = dico[i];
        while (p != NULL) {
            l=p->next;
            free(p->val);
            free(p);
            p=l;
        }
    }
}

