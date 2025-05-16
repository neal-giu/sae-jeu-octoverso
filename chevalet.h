/* src/chevalet.h */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef char Chevalet;

/* --- Liste chaînée de chevalets --- */
typedef struct _Maillon_ {
    Chevalet element;
    struct _Maillon_* suivant;
} Maillon;

typedef struct {
    Maillon* premier;
} Liste;

typedef struct {
    Liste* liste;
    Maillon* pred;
    Maillon* courant;
} Iterateur;

enum {
    MAX_CHEVALETS = 88,
    NB_CHEVALETS_DEBUT = 12,
    NB_CHEVALET_MOT_DEBUT = 4,
    NB_MAX_RAIL = 8,
    NB_MIN_RAIL = 2,
};

/* initialisation et gestion de Liste/Iterateur */
void initListe(Liste* li);
int estVide(const Liste* li);
int inclure(Liste* li, Chevalet it);
void detruireListe(Liste* li);
void initIterateur(Liste* li, Iterateur* iter);
int finiIt(const Iterateur* iter);
Chevalet courantIt(const Iterateur* iter);
void suivantIt(Iterateur* iter);
Chevalet effacer(Iterateur* iter);

/* opérations sur Deck et Pioche */
void melanger_pioche(Chevalet pioche[], int taille);
void remplirDeck(Liste* deck, Chevalet pioche[], int* taillePioche);
void trierDeck(Liste* deck);
void afficherDeck(const Liste* deck);
void afficher_etat(const Liste* d1, const Liste* d2, const Chevalet rail[], const Chevalet inv[]);

/* rail */
int initialiserRail(Chevalet rail[], const char* m1, const char* m2);
void creerInverseRail(const Chevalet rail[], Chevalet inv[]);

/* validation */
int validermotDico(const char* dicoFile, const char* mot);
int validermotDeck(const Liste* deck, const char* mot);
int motDejaJoue(const char* listeMots[], int nb, const char* mot);

/* insertion et expulsion */
void formerMot(Liste* deck, const char* mot);
int VerifierTout(const Chevalet rail[], const Liste* deck, const char* dicoFile,
                 const char* mot, int railUtilises);

/* échange */
int echange(Liste* deck, char lettre, Chevalet pioche[], int* taillePioche);
