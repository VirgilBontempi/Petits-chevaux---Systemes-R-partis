#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "Reseau.h"
#include "Serveur.h"

int main(int argc, char **argv)
{
	/* ----------------------------
       Initialisation des variables
       ----------------------------*/
    pid_t id;
    // Paramètres du programme
    int port, nbJoueursPartie, nbChevaux;
    // Nombre de joueurs connectés
    int nbJoueursConnectes;

    int num;

    port = atoi(argv[1]);
    nbJoueursPartie = atoi(argv[2]);
    nbChevaux = atoi(argv[3]);
    
    // Tableau de structure des processus, taille = nbJoueursPartie
    char tableauPipe[nbJoueursPartie];

    nbJoueursConnectes = 0;

    num = socketServer(port, TCP);

    // Saisie controlée nb joueurs de la partie
    if (nbJoueursPartie > 4 || nbJoueursPartie < 2) {
        printf("Une partie doit contenir entre 2 et 4 joueurs.");
        exit(0);
    }
    // Saisie controlée nb chevaux de la partie
    if (nbChevaux > 4 || nbChevaux < 2) {
        printf("Un joueur ne peut posséder que 2 à 4 chevaux dans son écurie.");
        exit(0);
    }

    /* -----------------
       Le serveur écoute
       -----------------*/
    int indice;
    // Tant que tous les joueurs ne sont pas connectés
    for (indice = 0; indice < nbJoueursPartie; indice++) {
        id = fork();
        if (id == 0) // Fils
        {
            int msgSock;
            msgSock = accept(num, NULL, NULL);
            write(msgSock, "Vous êtes le joueur : " + (indice + 1), 23);
            /*int joueursRestants;
            joueursRestants = nbJoueursPartie - nbJoueursConnectes;
            printf("Un joueur s'est connecté, il reste %d places.", joueursRestants);*/
            nbJoueursConnectes++;
        }

    }
    while (nbJoueursConnectes < nbJoueursPartie) {

    }
    printf("Tous les joueurs ont rejoint la partie");

    return 0;
}

