#ifndef _LISTE_H_
#define _LISTE_H_

typedef void* ELEMENT;      // un element est en fait l'adresse de cet élément

// description d'un maillon de liste chainée
struct maillon {
    ELEMENT val;            // l'élément inseré dans la chaine
    struct maillon * next;  // adresse du maillon suivant
};

typedef struct maillon * Liste;  // une liste est l'adresse d'un maillon


Liste creer_liste(void);

int est_vide( Liste l);

Liste ajout_tete(ELEMENT elt, Liste l);

Liste supp_tete(Liste l);

void visualiser(Liste l, void affichage(void*));

int compter(Liste l);

// retourne NULL si pas trouvé
Liste recherche(Liste l, void* el, int compare(void*,void*));

Liste ajout_trie(ELEMENT elt, Liste l, int compare(void*,void*));

void libere_liste(Liste l);

#endif
