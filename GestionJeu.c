/* ---------------------------------------------------------------------------
 * Test de l'affichage du plateau pour le jeu des petits chevaux en mode texte
 * Auteur     : Damien Genthial
 */

#include <stdio.h>
#include <stdlib.h>

#define GESTION_JEU_C
#include "GestionJeu.h"

// Retourne une chaîne correspondant à une équipe
char* toString(Team t) {
    char* res;
    switch (t) {
        case ROUGE: res = "Rouge"; break;
        case JAUNE: res = "Jaune"; break;
        case VERTE: res = "Verte"; break;
        case BLEUE: res = "Bleue"; break;
    }
    return res;
}

// Détermine le caractère à afficher en fonction de la position sur la grille
// Version pour une grille (presque) vide : 
//      1..56 = le tour normal, origine en haut à droite (rouge)
//      61..66 = escalier de l'équipe en haut à droite (rouge)
//      71..76 = escalier de l'équipe en bas à droite (magenta)
//      81..86 = escalier de l'équipe en bas à gauche (vert)
//      91..96 = escalier de l'équipe en haut à gauche (bleu)
char whichChar(int pos) {
    // Pour faire un essai d'affichage de case non vide :
    if (pos == 44) return ROUGE;
    if (pos == 64) return ROUGE;
    if (pos == 83) return VERTE;

    if (pos <= 56) {
        if ((pos % 14) == 1)
            return '=';
        else return '.';
    } else {
        return '0' + (pos % 10);
    }
}

// Nombre de chevaux restants à l'écurie pour une équipe donnée
int nbHorsesHome(Team t) {
    return 3;
}


