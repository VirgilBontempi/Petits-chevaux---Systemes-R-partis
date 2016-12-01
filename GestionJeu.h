/* ---------------------------------------------------------------------------
 * Gestion du jeu des petits chevaux.
 * Auteur(s) : 
 */

#include <stdbool.h>

#ifndef GESTION_JEU_H
#define GESTION_JEU_H

#ifndef GESTION_JEU_C
#define PUBLIC extern
#else
#define PUBLIC
#endif

#define NB_JOUEURS 4


// Constantes pour la gestion des équipes
typedef enum {ROUGE = 'R', JAUNE = 'J', VERTE = 'V', BLEUE = 'B'} Team;

typedef struct {
    bool estArrive, estDansEnclos, estDansEscalier, estDansParcours;
    int position;
} cheval;

typedef struct {
    Team couleur;
    cheval* ptChevaux;
    int nbChevaux;
} joueur;

typedef struct {
    int pipeIn[2], pipeOut[2];
    int numSock;
    joueur ptJoueur;
} structComCliServ;


PUBLIC joueur TabJoueurs[NB_JOUEURS];

// Retourne une chaîne correspondant à une équipe
PUBLIC char* toString(Team t);

// Détermine le caractère à afficher en fonction de la position sur la grille
// Version pour une grille vide : 
//      1..56 = le tour normal, origine en haut à droite (rouge)
//      61..66 = escalier de l'équipe en haut à droite (rouge)
//      71..76 = escalier de l'équipe en bas à droite (jaune)
//      81..86 = escalier de l'équipe en bas à gauche (vert)
//      91..96 = escalier de l'équipe en haut à gauche (bleu)
PUBLIC char whichChar(int pos);

// Nombre de chevaux restants à l'écurie pour une équipe donnée
PUBLIC int nbHorsesHome(Team t);

#undef PUBLIC
#endif	/* GESTION_JEU_H */
