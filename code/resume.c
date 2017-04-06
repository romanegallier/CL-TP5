#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include "liste.h"
#include "dictionnaire.h"
#include "document.h"

#define TAILLE_DICO 1000
#define TAILLE_STOP 100
#define TAILLE_LIGNE 1000
#define TAILLE_RESUME 5
#define NOMBRE_CARACTERES_MIN 1
#define DELIM " .,\"?!;:\n\r'[]«)(»"


/**
 * écrit le résumé du fichier filename
 * @filename le chemin du fichier original
 * @phrases la liste des phrases classées
 * @nb_phrases la taille finale du résumé
 */

void sauve_resume(char* filename, Liste phrases,int nb_phrases) {
    char* prefix = "res_";
    char* fichier = calloc(strlen(prefix)+strlen(filename)+1,sizeof(*fichier));

    char* directory = strrchr(filename,'/');
    if (directory) {
        char c= *(directory+1);
        *(directory+1)='\0';
        strcpy(fichier,filename);
        *(directory+1)=c;
        filename = directory+1;
    }

    strcat(fichier,prefix);
    strcat(fichier,filename);

    FILE *pf;
    char ligne[TAILLE_LIGNE];
    int nb_caracteres=0;

    pf = fopen(fichier, "wt");
    if( pf == NULL )
    {
        perror("Error: ");
        free(fichier);
        return;
    }

    Liste p = NULL;
    int i=0;
    for (i=0,p = phrases; p!=NULL&&i<nb_phrases; p = p->next,i++) {
        PHRASE* phr = ((PHRASE*) p->val);
        fprintf(pf,"%s",phr->texte);
    }
    fclose(pf);
    free(fichier);

}


/**
 * affiche les phrases du résumé avec leur score et la fréquence de chaque mot
 * @phrases la liste des phrases classées
 * @nb_phrases la taille finale du résumé
 * @dico la table de hachage contenant le vocabulaire des documents
 * @taille_dico la taille de la table de hachage
 */

void affiche_resume(Liste phrases,int nb_phrases,Liste dico[], int taille_dico) {
    Liste p = NULL;
    int i=0;
    puts("\n-------------------------\n");
    for (i=0,p = phrases; p!=NULL&&i<nb_phrases; p = p->next,i++) {
        PHRASE* phr = ((PHRASE*) p->val);
        printf("[%lf] %s\t",phr->score,phr->texte);
        Liste mots = phr->mots;
        for (; mots!=NULL; mots = mots->next) {
            char* cle= (char*) mots->val;
            ENTREE* e = get(cle, dico, taille_dico);
            if (e) printf("%s[%5.1lf] ",cle,e->tf);
        }
        puts("\n");
    }
}


/**
 * remet à jour le champ tf des entrées du vocabulaire
 * @document la liste des phrases du document
 * @dico la table de hachage contenant le vocabulaire des documents
 * @taille_dico la taille de la table de hachage
 */

void update_tf(Liste document, Liste dico[], int taille_dico) {
    Liste p = NULL;

    // nouveau document remise à zero des tf
    reset_tf_dico(dico,taille_dico);

    // calcul du tf
    for (p = document; p!=NULL; p = p->next) {
        Liste mots = NULL;
        for (mots = ((PHRASE*) p->val)->mots; mots!=NULL; mots = mots->next) {
            char* cle= (char*) mots->val;
            ENTREE* e = get(cle, dico, taille_dico);
            if (e) e->tf++;
        }
    }
}

/**
 * Prend une liste de phrases (c.-à-d. un document) en entrée et calcule le score pour chaque phrase.
 * @document la liste des phrases du document
 * @dico la table de hachage contenant le vocabulaire des documents
 * @taille_dico la taille de la table de hachage
 * @return Cette fonction retourne la liste triée par le score
 */

//TODO:
Liste classifie_phrase_tfidf(Liste document, Liste dico[], int taille_dico) {
    Liste p = NULL;
    Liste triee=NULL;
    update_tf(document, dico, taille_dico);

    // calcul des scores de phrase
    for (p = document; p!=NULL; p = p->next) {
        PHRASE* phr = ((PHRASE*) p->val);
        phr->score=0;
        Liste mots = NULL;
        for (mots = phr->mots; mots!=NULL; mots = mots->next) {
            char* cle= (char*) mots->val;
            ENTREE* e = get(cle, dico, taille_dico);
            if (e) phr->score+= (e->tf)?1+log(e->tf):0;// a changer 
        }
        triee = ajout_trie(phr, triee, compare_score);
//        affiche_resume(triee,TAILLE_RESUME);
    }
    return triee;
}


/**
 * Prend une liste de phrases (c.-à-d. un document) en entrée et calcule le score pour chaque phrase.
 * @document la liste des phrases du document
 * @dico la table de hachage contenant le vocabulaire des documents
 * @taille_dico la taille de la table de hachage
 * @return Cette fonction retourne la liste triée par le score
 */

Liste classifie_phrase_tf(Liste document, Liste dico[], int taille_dico) {
    Liste p = NULL;
    Liste triee=NULL;
    update_tf(document, dico, taille_dico);

    // calcul des scores de phrase
    for (p = document; p!=NULL; p = p->next) {
        PHRASE* phr = ((PHRASE*) p->val);
        phr->score=0;
        Liste mots = NULL;
        for (mots = phr->mots; mots!=NULL; mots = mots->next) {
            char* cle= (char*) mots->val;
            ENTREE* e = get(cle, dico, taille_dico);
            if (e) phr->score+= (e->tf)?1+log(e->tf):0;
        }
        triee = ajout_trie(phr, triee, compare_score);
//        affiche_resume(triee,TAILLE_RESUME);
    }
    return triee;
}


/**
 * Prend une liste de documents en entrée et calcule le score pour mot du dictionnaire.
 * @documents la liste des documents à partir desquels calculer l'idf
 * @nb_doc nombre de documents dans la collection
 * @dico la table de hachage contenant le vocabulaire des documents
 * @taille_dico la taille de la table de hachage
 */

//TODO: ajouter le calcul de l'idf
void calcule_idf(Liste documents[], int nb_doc, Liste dico[], int taille_dico) {
	Liste p = NULL;
	Liste d= NULL;
	int apparition;
	int idf;
	for (int i=0; i<nb_doc; i++){ //pour tout les documents
		apparition=0;
		for (d= dico[i]; d!=NULL; d=d-> next ){ // pour tous les mots du dictionaire 
			idf =0;
			
			for (p= documents[i]; p!=NULL; p=p->next){  // parcours du document 
				Liste mots = NULL;
				for (mots =((PHRASE*) p->val)->mots; mots!=NULL; mots = mots->next) {
					char* cle= (char*) mots->val;
					ENTREE* e = get(cle, dico, taille_dico);
					if (compare_entree(e,d->val)){
						apparition=1;
					}
				}
			}
			if (apparition){
					idf ++;
			}
			
			ENTREE* e = d->val;
			e->idf= log (nb_doc/idf)/ log (10);
        
		}
	}	
}



/**
 * lit une liste de mots à partir d'un fichier texte et charge un dictionnaire
 * @fichier le chemin du fichier
 * @dico la table de hachage contenant le dictionnaire à remplir
 * @taille_dico la taille de la table de hachage
 * @return 1 en cas de succès, -1 sinon
 */

int charge_liste_mots(char* fichier,Liste dico[], int taille_dico) {

    int l=0;
    FILE *pf;
    char ligne[TAILLE_LIGNE];

    pf = fopen(fichier, "rt");
    if( pf == NULL )
    {
        perror("Error: ");
        return -1;
    }
    // lecture ligne par ligne pour les traitements et la représentation
    while (fgets(ligne, TAILLE_LIGNE, pf)!=NULL) {
        char* mot = strtok (ligne,DELIM);
        if (mot !=NULL) {
            put(mot,dico,taille_dico);
        }
    }

    fclose(pf);
    return 1;
}



/**
 * lit le fichier passés en paramètres et initialise le dictionnaire
 * @fichier le chemin du fichier à lire
 * @documents le tableau de documents
 * @textes l'espace mémoire nécessaire pour les textes bruts
 * @i numéro du document à saisir à partir du fichier texte
 * @dico la table de hachage contenant le vocabulaire des documents
 * @taille_dico la taille de la table de hachage
 * @stops la table de hachage contenant les mots vides (stop words)
 * @taille_min le nombre minimum de caractères qu'une phrase doit contenir
 * @return 1 en cas de succès, -1 sinon
 */

int charge_documents(char* fichier, Liste documents[], char* textes[], int i, Liste dico[], int taille_dico, Liste stops[], int taille_stops, int taille_min) {

    int l=0;
    FILE *pf;
    char ligne[TAILLE_LIGNE];
    int nb_caracteres=0;

    pf = fopen(fichier, "rt");
    if( pf == NULL )
    {
        perror("Error: ");
        return -1;
    }

    // calcul du nombre de caractères dans le texte du document i
    fseek(pf, 0L, SEEK_END);
    nb_caracteres=ftell(pf);
    rewind(pf);
    nb_caracteres-=ftell(pf);

    // lecture de ce texte à partir du fichier
    textes[i] = calloc(nb_caracteres, sizeof(**textes));
    fread(textes[i], sizeof(**textes), nb_caracteres, pf);



    // lecture ligne par ligne pour les traitements et la représentation
    for (l=0,rewind(pf); fgets(ligne, TAILLE_LIGNE, pf)!=NULL; l++) {

        //DONE: ajouter une condition pour accepter la phrase
		if (strlen(ligne)>taille_min ){	
			PHRASE *p = calloc(1,sizeof(*p));
			p->texte = strdup(ligne);
			p->position = l;
			char* mot = strtok (ligne,DELIM);
			if (!mot) continue;
			lowercase(mot);
			int nb_mots=0;
			do {
				if (get(mot,stops,taille_stops)) 	continue;
				char* cle = put(mot,dico,taille_dico);
				nb_mots++;
				p->nb_mots=nb_mots;
				p->mots=ajout_tete(cle,p->mots);
			} while(NULL != (mot=strtok(NULL,DELIM)));
			
			documents[i] = ajout_tete(p,documents[i]);
		}
    }
    fclose(pf);
    return 1;
}


void print_usage(char * exec) {
    printf("utilisation %s [OPTION]... [FICHIER]... \n", exec);
    puts("Génère les résumés des FICHIER passés en entrée");
    puts("");
    puts("\t -l \t \t la longueur du résumé en nombre de phrases");
    puts("\t -c \t \t la taille minimale des résumés en nombre de caractères");
}



int main(int argc, char *argv[]) {
    int i,opt;
    int taille_resume=TAILLE_RESUME;
    int nb_caracteres=NOMBRE_CARACTERES_MIN;
    Liste dico[TAILLE_DICO]= {NULL}; // l'ensemble de mots des documents
    Liste stopwords[TAILLE_DICO]= {NULL}; // mots vides
    char** textes; // tableau des textes des documents
    Liste* documents; // l'ensemble des documents sous forme de tableau contenant des listes de phrases
    int nb_doc=0;

    // gestion des arguments
    if (argc < 2)
    {
        print_usage(argv[0]);
        exit(0);
    }

    while ((opt = getopt(argc, argv, "c:l:")) != -1) {
        switch(opt) {
        case 'l':
            puts(optarg);
            taille_resume = atoi(optarg);
            break;
        case 'c':
            puts(optarg);
            nb_caracteres= atoi(optarg);
            break;
        case '?':
            print_usage(argv[0]);
            exit(0);

        }
    }



    // chargement de la liste des mots vides
    charge_liste_mots("./french.list",stopwords,TAILLE_STOP);
    //   affiche_dico(stopwords,TAILLE_STOP);


    // phase de chargement : pour tous les documents on met le dico et le tableau de texte à jour
    nb_doc= argc-optind;
    textes = calloc(nb_doc, sizeof(*textes));
    documents = calloc(nb_doc, sizeof(*documents));

    for (i = 0 ; i <nb_doc; i++) {
        if (!charge_documents(argv[optind+i],documents,textes,i,dico,TAILLE_DICO,stopwords,TAILLE_STOP,nb_caracteres)) {
            printf("erreur de chargement pour le document %s", argv[optind+i]);
            exit(0);
        }
    }


// calcule_idf(documents,nb_doc,dico,TAILLE_DICO);

// affiche_dico(dico,TAILLE_DICO);


// phase d'ordonnancement et d'écriture du résumé
    for (i = 0 ; i <nb_doc; i++) {
        Liste phrases = classifie_phrase_tf(documents[i],dico,TAILLE_DICO);
        int nb_phrases = compter(documents[i]);
        affiche_resume(phrases,taille_resume,dico,TAILLE_DICO);
        sauve_resume(argv[optind+i],phrases,taille_resume);
        libere_liste(phrases);
    }

    libere_documents(documents, nb_doc);
    libere_dico(dico,TAILLE_DICO);
    for (i = 0 ; i <nb_doc; i++) {
        if (textes[i]!=NULL) free(textes[i]);
    }
    free(textes);
}
