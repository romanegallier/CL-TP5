Embryon de code pour effectuer un résumé automatique basé sur des mesures de fréquences et de tf-idf. 

liste.h : définitions des structures de données pour gérer une liste chaînée.
dictionnaire.h : définition de la notion d'"entrée" d'un dictionnaire. Un dictionnaire est tout simplement un tableau de liste qui contiennent des entrées. 
documents.h : définition de la notion de "phrase" d'un document. Un document est une liste de phrases. 
Chaque phrase contient des informations utiles telles que la position orginale de la phrase dans le texte, la liste des mots qui la composent, le score pour l'ordonnancement etc.  

liste.c : contient les fonctions de base pour gérer des listes en C. 
dictionnaire.c : contient les fonctions de base pour manipuler une table d'association qui ici contiendra les mots et différentes valeurs définies dans dictionnaire.h
document.c : contient un ensemble de fonctions pour manipuler des phrases et faire des calculs sur celles-ci.
resume.c : contient le programme principal et les fonctions nécessaires à la mise en œuvre du résumé automatique. 
 

resume.c est le SEUL endroit où des modifications doivent être apportées.

Pour compiler : 
 make 

Pour exécuter : 
./resume [OPTION]... [FICHIER]...

Pour l'aide :
./resume 

Pour créer une archive: 
make archive


ATTENTION: 
le code présenté n'a pas pour vocation à être un système à l'état de l'art. Il y a notamment un ensemble de simplifications qui rendent le code non-optimal mais permettent de le rendre compréhensible. 

La représentation des phrases n'est pas optimale. En effet, la liste des mots devrait être un ensemble strict où chaque mot unique devrait être une structure avec sa forme et son nombre d'occurrences dans la phrase. 

UTF-8 : les fonctions de gestion de chaînes de caractères utilisées sont les fonctions classiques attendant de l'ASCII. Il existe donc une incertitude sur le traitement des caractères accentués notamment vis-à-vis de la casse et de la comparaison (pas de normalisation appliquée). 

