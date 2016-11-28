/* ---------------------------------------------------------------------------
 * Affichage du plateau de jeu, utilise GestionJeu pour déterminer le 
 * caractère à afficher à une position donnée
 * Auteur     : Damien Genthial
12345678901234567890123456789           
            o o =
            o 1 o
            o 2 o
    B B     o 3 o    R R 
    B B     o 4 o    R R 
            o 5 o
= o o o o o o 6 o o o o o o o
o 1 2 3 4 5 6   6 5 4 3 2 1 o
o o o o o o o 6 o o o o o o =
            o 5 o
    V V     o 4 o    J J
    V V     o 3 o    J J 
            o 2 o
            o 1 o
            = o o
 */

#include <stdio.h>
#include "GestionJeu.h"

#define PLATEAU_C
#include "Plateau.h"

static const char CSI[] = {0x1B, '[', 0};

static void clear() { printf("%s2J", CSI); }
static void jumpTo(int row, int column) { printf("%s%d;%dH", CSI, row, column); }
static void up(int nb) { printf("%s%dA", CSI, nb); }
static void down(int nb) { printf("%s%dB", CSI, nb); }
static void right(int nb) { printf("%s%dC", CSI, nb); }
static void left(int nb) { printf("%s%dD", CSI, nb); }
// static void black(void) { printf("%s1;30m", CSI); }
static void red(void) { printf("%s1;31m", CSI); }
static void green(void) { printf("%s1;32m", CSI); }
 static void yellow(void) { printf("%s1;33m", CSI); }
static void blue(void) { printf("%s1;34m", CSI); }
//static void magenta(void) { printf("%s1;35m", CSI); }
// static void cyan(void) { printf("%s1;36m", CSI); }
// static void white(void) { printf("%s1;37m", CSI); }
static void normal(void) { printf("%s0m", CSI); }

// Caractère à utiliser pour afficher les chevaux d'une équipe donnée
static char whichCharHorse(Team t) {
    char res;
    switch (t) {
        case ROUGE: res = 'R'; break;
        case JAUNE: res = 'J'; break;
        case VERTE: res = 'V'; break;
        case BLEUE: res = 'B'; break;
    }
    return res;
}

// Type pour toutes les fonctions de sélection de la couleur d'affichage
typedef void (*SelectColor) (void);
// Variable pour la couleur par défaut à un instant donné
SelectColor defaultColor = normal;

// Affichage d'un caractère dans la couleur par défaut ou dans la couleur de l'équipe
// s'il s'agit d'un cheval d'une équipe
static void putColoredChar(char c) {
    switch (c) {
        case ROUGE: red(); break;
        case JAUNE: yellow(); break;
        case VERTE: green(); break;
        case BLEUE: blue(); break;
    }
    putchar(c); defaultColor();
}

// Les origines permettent de décaler un peu le plateau dans la fenêtre
void affichePlateau(int originRow, int originColumn) {
    int i;
    int pos = 1;

    clear();
    jumpTo(originRow,originColumn + 17);

    red(); defaultColor = red;
    for (i = 0; i < 7; i++) { putColoredChar(whichChar(pos++)); left(1); down(1); }
    up(1); right(1);
    for (i = 0; i < 6; i++) { putchar(' '); putColoredChar(whichChar(pos++)); }
    down(1); left(1); putColoredChar(whichChar(pos++)); left(1); down(1);

    yellow(); defaultColor = yellow;
    for (i = 0; i < 6; i++) { putColoredChar(whichChar(pos++)); left(3); }
    for (i = 0; i < 7; i++) { putColoredChar(whichChar(pos++)); left(1); down(1); }
    up(1); left(2); putColoredChar(whichChar(pos++)); left(3);

    green(); defaultColor = green;
    for (i = 0; i < 7; i++) { putColoredChar(whichChar(pos++)); left(1); up(1); }
    down(1); left(2);
    for (i = 0; i < 6; i++) { putColoredChar(whichChar(pos++)); left(3); }
    up(1); right(2); putColoredChar(whichChar(pos++)); left(1); up(1);

    blue(); defaultColor = blue;
    for (i = 0; i < 7; i++) { putColoredChar(whichChar(pos++)); putchar(' '); }
    left(2); up(1);
    for (i = 0; i < 5; i++) { putColoredChar(whichChar(pos++)); left(1); up(1); }
    putColoredChar(whichChar(pos++)); putchar(' '); putColoredChar(whichChar(pos++));

    // Escaliers
    left(1); down(1);
    red(); pos = 61;
    for (i = 0; i < 6; i++) { putchar(whichChar(pos++)); left(1); down(1); }
    
    right(12);
    yellow(); pos = 71;
    for (i = 0; i < 6; i++) { putchar(whichChar(pos++)); left(3); }
    
    down(6);
    green(); pos = 81;
    for (i = 0; i < 6; i++) { putchar(whichChar(pos++)); left(1); up(1); }
    
    left(12);
    blue(); pos = 91;
    for (i = 0; i < 6; i++) { putchar(whichChar(pos++)); putchar(' '); }

    // Écuries
    jumpTo(originRow + 3, originColumn + 21);
    red();
    for (i = 0; i < nbHorsesHome(ROUGE); i++) {
        if (i == 2) { down(1); left(4); }
        putchar(whichCharHorse(ROUGE)); putchar(' ');
    }
    jumpTo(originRow + 10, originColumn + 21);
    yellow();
    for (i = 0; i < nbHorsesHome(JAUNE); i++) {
        if (i == 2) { down(1); left(4); }
        putchar(whichCharHorse(JAUNE)); putchar(' ');
    }
    jumpTo(originRow +10, originColumn + 6);
    green();
    for (i = 0; i < nbHorsesHome(VERTE); i++) {
        if (i == 2) { down(1); left(4); }
        putchar(whichCharHorse(VERTE)); putchar(' ');
    }
    jumpTo(originRow + 3, originColumn + 6);
    blue();
    for (i = 0; i < nbHorsesHome(BLEUE); i++) {
        if (i == 2) { down(1); left(4); }
        putchar(whichCharHorse(BLEUE)); putchar(' ');
    }

    normal(); jumpTo(23, 1);
}

