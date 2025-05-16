/* src/main.c */
#include "chevalet.h"

int main(){
    srand((unsigned)time(NULL));
    /* --- Initialisation --- */
    Chevalet pioche[MAX_CHEVALETS] = {
        'A','A','A','A','A','A','A','A','A',
        'B','C','C','D','D','D',
        'E','E','E','E','E','E','E','E','E','E','E','E','E','E',
        'F','G','H',
        'I','I','I','I','I','I','I',
        'J',
        'L','L','L','L','L',
        'M','M','M','N','N','N','N','N','N',
        'O','O','O','O','O',
        'P','P','Q',
        'R','R','R','R','R','R',
        'S','S','S','S','S','S','S',
        'T','T','T','T','T','T',
        'U','U','U','U','U',
        'V','V'
    };
    int TaillePioche = MAX_CHEVALETS;
    Liste Deck1, Deck2;
    initListe(&Deck1); initListe(&Deck2);
    melanger_pioche(pioche, TaillePioche);
    remplirDeck(&Deck1, pioche, &TaillePioche);
    remplirDeck(&Deck2, pioche, &TaillePioche);
    trierDeck(&Deck1); trierDeck(&Deck2);

    const char* Dico = "dico.txt";

    /* Initialisation du rail */
    Chevalet rail[NB_MAX_RAIL], inv[NB_MAX_RAIL];
    char m1[16], m2[16];
    do { printf("1> "); scanf("%15s", m1); } 
    while(strlen(m1)!=4 || !validermotDico(Dico,m1) || !validermotDeck(&Deck1,m1));
    do { printf("2> "); scanf("%15s", m2); } 
    while(strlen(m2)!=4 || !validermotDico(Dico,m2) || !validermotDeck(&Deck2,m2));

    formerMot(&Deck1,m1); formerMot(&Deck2,m2);
    initialiserRail(rail,m1,m2);
    creerInverseRail(rail,inv);
    getchar();  /* vidage du '\n' de la dernière saisie */

    /* Suivi des mots joués et du dernier coup */
    const char* motsJoues[MAX_CHEVALETS];
    int nbJ = 0;
    char lastMot[16] = ""; 
    int lastLen = 0, lastRailU = 0;
    Chevalet railAvant[NB_MAX_RAIL];
    Liste deckAvant;
    initListe(&deckAvant);
    int challenged = 0;

    /* --- Boucle principale --- */
    while(1){
        if(estVide(&Deck1)||estVide(&Deck2)) break;
        afficher_etat(&Deck1,&Deck2,rail,inv);
        printf("%d> ", 1);
        char ligne[64];
        if(!fgets(ligne,sizeof ligne,stdin)) break;
        if(strlen(ligne)<2) continue;

        /* Échange */
        if(ligne[0]=='-' && ligne[1]){
            if(echange(&Deck1, ligne[1], pioche, &TaillePioche))
                trierDeck(&Deck1);
            continue;
        }

        /* Challenge */
        if(ligne[0]=='r'||ligne[0]=='v'){
            if(!challenged && lastLen==NB_MAX_RAIL){
                /* vérifier jouabilité sur état d'avant */
                if(VerifierTout(railAvant, &deckAvant, Dico, lastMot, lastRailU)){
                    /* pénalité : adversaire défausse un chevalet */
                    if(!estVide(&Deck2)){
                        Iterateur it; initIterateur(&Deck2,&it);
                        effacer(&it);
                    }
                    motsJoues[nbJ++] = strdup(lastMot);
                }
            }
            challenged = 1;
            continue;
        }

        /* Coup normal */
        char cote = ligne[0];
        if(cote!='R' && cote!='V') continue;
        char* p = strchr(ligne,'(');
        char* q = strchr(ligne,')');
        if(!p||!q||q<p) continue;
        int railU = q-p-1;
        if(railU<NB_MIN_RAIL) continue;

        char mot[16]={0};
        int idx=0;
        for(char* t=ligne+1; t<ligne+p; ++t)
            if(*t!=' '&&*t!='\t') mot[idx++]=*t;
        for(char* t=p+1; t<q; ++t) mot[idx++]=*t;
        for(char* t=q+1; *t && *t!=' '&&*t!='\n'; ++t) mot[idx++]=*t;
        mot[idx]=0;
        if(motDejaJoue(motsJoues,nbJ,mot)) continue;
        if(!VerifierTout(rail, &Deck1, Dico, mot, railU)) continue;

        /* Sauvegarde pour challenge */
        memcpy(railAvant, rail, sizeof rail);
        detruireListe(&deckAvant);
        initListe(&deckAvant);
        { Iterateur it; initIterateur(&Deck1,&it);
          while(!finiIt(&it)){ inclure(&deckAvant,courantIt(&it)); suivantIt(&it);} }
        strcpy(lastMot,mot);
        lastLen = strlen(mot);
        lastRailU = railU;
        challenged = 0;

        /* Application du coup : insertion simplifiée */
        /* Expulsion de railU lettres vers l'adversaire */
        for(int i=0;i<railU;i++){
            Chevalet expuls = rail[NB_MAX_RAIL-1-i];
            inclure(&Deck2, expuls);
        }
        /* Glissement et insertion */
        if(cote=='R'){
            memmove(rail+railU, rail, (NB_MAX_RAIL-railU)*sizeof(Chevalet));
            memcpy(rail, mot, railU*sizeof(Chevalet));
        } else {
            /* sur rail inverse */
            Chevalet tmp[NB_MAX_RAIL];
            creerInverseRail(rail,tmp);
            memmove(tmp+railU, tmp, (NB_MAX_RAIL-railU)*sizeof(Chevalet));
            memcpy(tmp, mot, railU*sizeof(Chevalet));
            creerInverseRail(tmp,rail);
        }

        /* Retirer du deck du joueur les lettres jouées */
        formerMot(&Deck1, mot);

        /* Bonus 8 lettres : s'il a utilisé 8 du rail */
        if(lastLen==NB_MAX_RAIL){
            printf("-1> ");
            char choix;
            if(scanf(" %c",&choix)==1){
                echange(&Deck1, choix, pioche, &TaillePioche);
                trierDeck(&Deck1);
            }
            getchar();
        }

        motsJoues[nbJ++] = strdup(mot);
        creerInverseRail(rail,inv);
    }

    /* fin propre */
    detruireListe(&Deck1);
    detruireListe(&Deck2);
    detruireListe(&deckAvant);
    return 0;
}
