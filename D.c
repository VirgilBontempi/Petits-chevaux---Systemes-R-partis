#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "D.h"

/*
int main(int argc, char **argv) {
    // Variables
    int valeurDe;
    
    // Récupération de la valeur du dé
    valeurDe = lancerDe();
    // Affichage du dé obtenu
    afficheResDe(valeurDe);
}
*/

/* ---------------------
 * Simule un lancé de dé
 * ---------------------*/
int lancerDe() {
    srand(time(NULL));
    return (rand() % 6) + 1;
}

/* --------------------
 * Affiche le dé obtenu
 * --------------------*/
void afficheResDe(int valeur) {
    switch (valeur) {
        case 1:

            printf("┌───────┐\n");
            printf("│       │\n");
            printf("│   O   │\n");
            printf("│       │\n");
            printf("└───────┘\n");
            break;
        case 2:
            printf("┌───────┐\n");
            printf("│     O │\n");
            printf("│       │\n");
            printf("│ O     │\n");
            printf("└───────┘\n");
            break;
        case 3:
            printf("┌───────┐\n");
            printf("│     O │\n");
            printf("│   O   │\n");
            printf("│ O     │\n");
            printf("└───────┘\n");
            break;
        case 4:
            printf("┌───────┐\n");
            printf("│ O   O │\n");
            printf("│       │\n");
            printf("│ O   O │\n");
            printf("└───────┘\n");
            break;
        case 5:
            printf("┌───────┐\n");
            printf("│ O   O │\n");
            printf("│   O   │\n");
            printf("│ O   O │\n");
            printf("└───────┘\n");
            break;
        case 6:
            printf("┌───────┐\n");
            printf("│ O   O │\n");
            printf("│ O   O │\n");
            printf("│ O   O │\n");
            printf("└───────┘\n");
            break;
    }
}
