/* src/chevalet.c */
#include "chevalet.h"

/* -------- Liste chaînée -------- */
void initListe(Liste* li) { li->premier = NULL; }
int estVide(const Liste* li) { return li->premier == NULL; }
int inclure(Liste* li, Chevalet it) {
    Maillon* m = malloc(sizeof *m);
    if (!m) return 0;
    m->element = it;
    m->suivant = NULL;
    if (li->premier == NULL) li->premier = m;
    else {
        Maillon* t = li->premier;
        while (t->suivant) t = t->suivant;
        t->suivant = m;
    }
    return 1;
}
void detruireListe(Liste* li) {
    Maillon* m = li->premier;
    while (m) {
        Maillon* nx = m->suivant;
        free(m);
        m = nx;
    }
    li->premier = NULL;
}

/* itérateur */
void initIterateur(Liste* li, Iterateur* it) {
    it->liste = li; it->pred = NULL; it->courant = li->premier;
}
int finiIt(const Iterateur* it) { return it->courant == NULL; }
Chevalet courantIt(const Iterateur* it) { return it->courant->element; }
void suivantIt(Iterateur* it) {
    it->pred = it->courant;
    it->courant = it->courant->suivant;
}
Chevalet effacer(Iterateur* it) {
    Maillon* sup = it->courant;
    Chevalet c = sup->element;
    if (it->pred) it->pred->suivant = sup->suivant;
    else it->liste->premier = sup->suivant;
    it->courant = sup->suivant;
    free(sup);
    return c;
}

/* Mélange Fisher–Yates */
void melanger_pioche(Chevalet pioche[], int taille) {
    for (int i = taille-1; i > 0; --i) {
        int j = rand() % (i+1);
        Chevalet t = pioche[i];
        pioche[i] = pioche[j];
        pioche[j] = t;
    }
}
void remplirDeck(Liste* deck, Chevalet pioche[], int* taillePioche) {
    for(int i=0;i<NB_CHEVALETS_DEBUT;i++){
        inclure(deck, pioche[--(*taillePioche)]);
    }
}

/* Tri alphabétique */
static int cmplet(const void* a, const void* b) {
    return *(const Chevalet*)a - *(const Chevalet*)b;
}
void trierDeck(Liste* deck) {
    int n=0;
    Chevalet tmp[MAX_CHEVALETS];
    Iterateur it; initIterateur(deck, &it);
    while(!finiIt(&it)) { tmp[n++] = courantIt(&it); suivantIt(&it); }
    if (n<2) return;
    qsort(tmp,n,sizeof *tmp,cmplet);
    detruireListe(deck);
    for(int i=0;i<n;i++) inclure(deck,tmp[i]);
}
void afficherDeck(const Liste* deck) {
    Iterateur it; initIterateur((Liste*)deck, &it);
    while(!finiIt(&it)) {
        putchar(courantIt(&it));
        suivantIt(&it);
    }
    putchar('\n');
}

/* État du jeu */
void afficher_etat(const Liste* d1,const Liste* d2,const Chevalet rail[],const Chevalet inv[]){
    printf("1 : "); afficherDeck(d1);
    printf("2 : "); afficherDeck(d2);
    printf("R : ");
    for(int i=0;i<NB_MAX_RAIL;i++) putchar(rail[i]);
    printf("\nV : ");
    for(int i=0;i<NB_MAX_RAIL;i++) putchar(inv[i]);
    putchar('\n');
}

/* Rail initial et inverse */
int initialiserRail(Chevalet rail[], const char* m1, const char* m2){
    if (strlen(m1)!=NB_CHEVALET_MOT_DEBUT||strlen(m2)!=NB_CHEVALET_MOT_DEBUT)
        return 0;
    if (strcmp(m1,m2)<0){
        memcpy(rail,m1,4); memcpy(rail+4,m2,4);
    } else {
        memcpy(rail,m2,4); memcpy(rail+4,m1,4);
    }
    return 1;
}
void creerInverseRail(const Chevalet rail[], Chevalet inv[]){
    for(int i=0;i<NB_MAX_RAIL;i++) inv[i] = rail[NB_MAX_RAIL-1-i];
}

/* Validation dictionnaire (lecture ligne à ligne) */
int validermotDico(const char* dicoFile, const char* mot){
    FILE* f = fopen(dicoFile,"r");
    if(!f) return 0;
    char buf[64];
    int ok=0;
    while(fgets(buf,sizeof(buf),f)){
        buf[strcspn(buf,"\r\n")] = '\0';
        if (strcmp(buf,mot)==0){ ok=1; break; }
    }
    fclose(f);
    return ok;
}

/* Validation Deck (lettres disponibles) */
int validermotDeck(const Liste* deck, const char* mot){
    char tmp[MAX_CHEVALETS];
    int n=0;
    Iterateur it; initIterateur((Liste*)deck,&it);
    while(!finiIt(&it)){ tmp[n++]=courantIt(&it); suivantIt(&it); }
    for(const char* p=mot;*p;p++){
        int t=0;
        for(int i=0;i<n;i++) if(tmp[i]==*p){ tmp[i]=0; t=1; break;}
        if(!t) return 0;
    }
    return 1;
}

/* Mots déjà joués */
int motDejaJoue(const char* listeMots[], int nb, const char* mot){
    for(int i=0;i<nb;i++)
        if(strcmp(listeMots[i],mot)==0) return 1;
    return 0;
}

/* Forme un mot (retire du deck) */
void formerMot(Liste* deck, const char* mot){
    for(const char*p=mot;*p;p++){
        Iterateur it; initIterateur(deck,&it);
        while(!finiIt(&it)){
            if(courantIt(&it)==*p){ effacer(&it); break;}
            suivantIt(&it);
        }
    }
}

/* Validation mot complet (rail+deck), railUtilises = nombre de lettres prises dans le rail */
int VerifierTout(const Chevalet rail[], const Liste* deck, const char* dicoFile,
                 const char* mot, int railUtilises){
    int L = strlen(mot);
    if(L>NB_MAX_RAIL || L<NB_MIN_RAIL) return 0;
    if(railUtilises<NB_MIN_RAIL) return 0;
    if(!validermotDico(dicoFile,mot)) return 0;
    if(!validermotDeck(deck,mot)) return 0;
    return 1;
}

/* Échange chevalet (commande "-X") */
int echange(Liste* deck, char lettre, Chevalet pioche[], int* taillePioche){
    Iterateur it; initIterateur(deck,&it);
    while(!finiIt(&it)){
        if(courantIt(&it)==lettre){
            effacer(&it);
            inclure(deck, pioche[--(*taillePioche)]);
            return 1;
        }
        suivantIt(&it);
    }
    return 0;
}
