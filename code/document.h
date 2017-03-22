#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#define TAILLE_CHAINE 50
#include "liste.h"
// description d'une phrase
typedef struct phrase {
    int position;  // position de la phrase dans le fichier original
    char * texte;  // texte brut de la phrase
    Liste mots;    // liste des mots de la phrase
    int nb_mots;   // nombre total de mots dans la liste
    double score;  // score de cette phrase pour l'ordonnancement
} PHRASE;          // PHRASE est le type de cette structure

double tf(char* mot, PHRASE* phr);

int compare_mot(void* mot1, void* mot2);

int compare_token_phrase(void* token, void* phrase);

int compare_score(void* phrase1, void* phrase2);

void lowercase(char* chaine);

double similarite(PHRASE* phr1, PHRASE* phr2, Liste dico[], int taille_dico);

double** creer_matrice(int taille);

void remplir_matrice(Liste document, double** matrice, int taille, Liste dico[], int taille_dico);

void affiche_matrice(double** matrice, int taille);

void libere_documents(Liste* documents, int taille);

#endif
