#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "liste.h"
#include "dictionnaire.h"
#include "document.h"


double tf(char* mot, PHRASE* phr) {

    double tf =0;
    char * p = phr->texte;

    while ((p=strstr(p,mot))!=NULL) {
        p++;
        tf++;
    }
    return tf;
}



int compare_mot(void* mot1, void* mot2) {
    char * m1 = (char*) mot1;
    char * m2 = (char*) mot2;
    if (!mot1) return -1;
    if (!mot2) return 1;
    return strcasecmp(m1,m2);
}


int compare_token_phrase(void* token, void* phrase) {
    if (token && phrase && ((PHRASE *)phrase)->texte) {
        char* trouve = strcasestr(((PHRASE *)phrase)->texte, token);
        if (NULL != strcasestr(((PHRASE *)phrase)->texte, token))
            return 0;
    }
    return 1;
}

int compare_score(void* phrase1, void* phrase2) {

    if (! phrase1) return 1;
    if (! phrase2) return -1;

    if (((PHRASE *)phrase1)->score < ((PHRASE *)phrase2)->score) return 1;
    else if (((PHRASE *)phrase1)->score > ((PHRASE *)phrase2)->score) return -1;
    return 0;

}





void lowercase(char* chaine) {
    int i;
    if (!chaine) return;
    int taille = strlen(chaine);
    for (i = 0 ; i < taille; i++) {
        chaine[i] = tolower(chaine[i]);
    }
}



double similarite(PHRASE* phr1, PHRASE* phr2, Liste dico[], int taille_dico) {

    double sim =0.0;

    double cross =0,tfidf1=0,tfidf2=0;
    Liste ouverts=NULL;
    Liste p= NULL;

    for (p= phr1->mots; p!=NULL; p=p->next) {

        if (recherche(ouverts,p->val,compare_mot)==NULL) {
            int tf1 = tf(p->val, phr1);
            int tf2 = tf(p->val, phr2);
            ENTREE* e = get(p->val, dico, taille_dico);
            double idf = (e)? e->idf: 0;
            if (strcasestr(phr2->texte,p->val)) {
                cross+=tf1*tf2*idf*idf;
            }
            tfidf1+= tf1*tf1*idf*idf;
            tfidf2+= tf2*tf2*idf*idf;
            ouverts= ajout_tete(p->val,ouverts);
        }
    }

    for (p= phr2->mots; p!=NULL; p=p->next) {
        if (recherche(ouverts,p->val,compare_mot)==NULL) {
            int tf2 = tf(p->val, phr2);
            ENTREE* e = get(p->val, dico, taille_dico);
            double idf = (e)? e->idf: 0;
            tfidf2+= tf2*tf2*idf*idf;
            ouverts= ajout_tete(p->val,ouverts);
        }
    }



    while(ouverts!=NULL)
        ouverts = supp_tete(ouverts);

    if (cross!=0) sim = cross/ (sqrt(tfidf2)* sqrt(tfidf1));


    return sim;

}

double** creer_matrice(int taille) {
    int i;
    double** m = calloc(taille, sizeof(*m));

    m[0] = calloc(taille*taille, sizeof(**m));

    for (i=1; i<taille; i++) {
        m[i]=m[i-1]+taille;
    }
    return m;
}


void remplir_matrice(Liste document, double** matrice, int taille, Liste dico[], int taille_dico) {
    int i,j;
    Liste c=NULL,p=NULL;
    printf("taille phrases %d\n",taille);
    int pos_max=0;
    for (p = document; p!=NULL; p=p->next) {
        PHRASE * phr1 =(PHRASE*)(p->val);
        i = phr1->position;
        for (c=p; c!=NULL; c=c->next) {
            PHRASE * phr2 =(PHRASE*)(c->val);
            j = phr2->position;
            if (pos_max<j) pos_max=j;
            matrice[i][j]= matrice[j][i]= similarite(phr1,phr2,dico,taille_dico);
        }
    }
    printf("pos max %d\n",pos_max);
}


void affiche_matrice(double** matrice, int taille) {
    int i,j;

    printf("     ");
    for (i = 0; i<taille; i++) {
        printf("%4d,",i);
    }
    puts("");
    for (i = 0; i<taille; i++) {
        printf("%4d,",i);
        for (j = 0; j<taille; j++) {
            printf("%4.3lf,",matrice[i][j]);
        }
        puts("");
    }
}

void libere_document(Liste document) {
    Liste p =NULL;
    for(p= document; p!=NULL; p=p->next) {
        if (((PHRASE *) p->val)->texte) free(((PHRASE *) p->val)->texte);
        libere_liste(((PHRASE *) p->val)->mots);
        free(p->val);
    }
    libere_liste(document);
}

void libere_documents(Liste* documents, int taille) {
    int i;
    for(i=0; i<taille; i++) {
        libere_document(documents[i]);
    }
    free(documents);
}

