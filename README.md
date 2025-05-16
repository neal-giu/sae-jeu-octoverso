# SAE S1.02 – Octo-Verso (BUT Informatique)

Ce projet a été réalisé dans le cadre de la SAE S1.02 du BUT Informatique (1ʳᵉ année).  
L’objectif est de développer une **application C** permettant à deux joueurs de s’affronter à Octo-Verso, en respectant toutes les règles : distribution des chevalets, formation de mots sur un rail recto/verso, échanges, challenges, bonus 8 lettres, défausse, etc.

---

## 🧠 Objectifs du projet

- Distribuer 12 chevalets à chaque joueur et gérer la pioche.  
- Implémenter la formation de mots sur un rail recto/verso (avec gestion des parenthèses).  
- Gérer les échanges de chevalets (`-X`).  
- Mettre en place le bonus pour mot de 8 lettres (défausse d’un chevalet).  
- Autoriser le **challenge** d’un mot de 8 lettres (`r`/`v`).  
- Assurer la robustesse de la saisie (rejets silencieux).  
- Terminer automatiquement la partie quand un deck devient vide.  
- Charger et valider les mots depuis `dico.txt`.  

Le jeu se déroule **en ligne de commande**, avec des invites de la forme `1>`, `2>`, etc.

---

## ⚙️ Fonctionnalités implémentées

| Commande  | Fonction                                                                               |
|-----------|----------------------------------------------------------------------------------------|
| `R(... )` | Joue un mot sur le rail **recto** (utilise des lettres du rail + du deck du joueur).  |
| `V(... )` | Joue un mot sur le rail **verso**.                                                     |
| `-X`      | Échange la lettre `X` du deck avec une lettre de la pioche.                            |
| `r` / `v` | **Challenge** : signalement d’un mot de 8 lettres qu’il aurait été possible de jouer.  |
| fin       | La partie se termine automatiquement quand l’un des decks devient vide.               |

---

## 🏗️ Organisation du projet

```text
/
├── src/
│   ├── chevalet.h      # Déclarations (listes, prototypes, constantes)
│   ├── chevalet.c      # Implémentations : deck, rail, validation, échange
│   └── main.c          # Boucle de jeu, saisie, affichage et logique générale
├── dico.txt            # Fichier de dictionnaire (mots valides)
└── README.md           # Ce fichier
```
---

## 🧪 Exemple

```
1> ABLE
2> NOIR

1 : [Deck1 trié]  
2 : [Deck2 trié]  
R(ABLE)          // Joue “ABLE” avec 4 lettres du rail  
-1> E            // Échange la lettre ‘E’  
r               // Challenge d’un mot de 8 lettres  
…               // La partie continue jusqu’à ce qu’un deck soit vide
```

---

## 📄 Licence

Ce projet est sous licence MIT. Voir le fichier [LICENSE](./LICENSE) pour plus d’informations.

---

## ✍️ Auteur

Réalisé par **Neal Giustignano**, étudiant en BUT Informatique.

---

## 🔗 Liens utiles

- Sujet de la SAE : [voir le PDF du sujet](./sujet-s1.pdf)
