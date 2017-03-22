#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "liste.h"

Liste creer_liste(void) {
    return NULL;
}

int est_vide( Liste l) {
    return l == NULL;
}

Liste ajout_tete(ELEMENT elt, Liste l) {
    Liste pm = calloc(1, sizeof( * pm )) ;
    if(pm == NULL) {
        printf("erreur\n");
        return NULL;
    }
    pm->val = elt;
    pm->next = l;
    return pm;
}

Liste ajout_trie(ELEMENT elt, Liste l, int compare(void*,void*)) {
    Liste p= NULL;

    if (l==NULL || compare((void*)elt, l->val)<0)
        return ajout_tete(elt, l);

    for(p= l; p->next!=NULL && compare((void*)elt, p->next->val)>0; p=p->next);
    p->next = ajout_tete(elt, p->next);

    return l;
}



void visualiser(Liste l, void affichage(void*)) {
    Liste p = NULL; // toujours bien d'initialiser les pointeurs dès leur déclaration...
    // cas particulier
    if(est_vide(l)) {
        return;
    }
    // cas général
    for(p = l ; p != NULL ; p=p->next) {
        printf(" -> ");
        affichage( p->val );
    }
}


int compter(Liste l) {
    Liste p = NULL;
    int nb = 0;
    p = l;
    while( p != NULL) {
        nb ++ ;
        p = p->next;
    }
    return nb;
}


Liste supp_tete(Liste l) {
    Liste pnext = NULL; // toujours bien d'initialiser les pointeurs dès leur déclaration...

    // cas particulier
    if(est_vide(l)) {
        return NULL;
    }
    // cas général. On a besoin de stocker l'adresse du maillon suivant
    pnext = l->next;
    free(l);
    return pnext;
}

// retourne NULL si pas trouvé
Liste recherche(Liste l, void* el, int compare(void*,void*)) {
    Liste p = l;
    while(!est_vide(p) ) { // ou tant que (p != NULL)
        if(compare(el, p->val) == 0) {
            return p; // trouvé => on sort en retournant le maillon
        }
        p = p->next;
    }
    // pas trouvé
    return NULL;
}

void libere_liste(Liste l) {
    while (l!=NULL)
        l=supp_tete(l);
}
