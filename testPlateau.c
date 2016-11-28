/* ---------------------------------------------------------------------------
 * Test de l'affichage du plateau pour le jeu des petits chevaux en mode texte
 * Auteur     : Damien Genthial
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "Plateau.h"
#include "GestionJeu.h"

int main(int nbArgs, char* arg[])
{
    // Pour un centrage dans une fenêtre 80x24
    affichePlateau(5, 24);

    // Pour l'exemple, si on a besoin de parcourir la liste des équipes ?
    Team t;
    printf("Équipes :\n");
    // N'affiche rien car 'R' > 'B'
    for (t = ROUGE; t <= BLEUE; t++) {
        printf("\t%s\n", toString(t));
    }

    char equipes[] = {ROUGE, JAUNE, VERTE, BLEUE};
    int i;
    printf("Équipes :\n");
    for (i = 0; i < 4; i++) {
        printf("\t%s\n", toString(equipes[i]));
    }
    return EXIT_SUCCESS;
}

