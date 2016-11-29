#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
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
        printf("Une partie doit contenir entre 2 et 4 joueurs.\n");
        exit(0);
    }
    // Saisie controlée nb chevaux de la partie
    if (nbChevaux > 4 || nbChevaux < 2) {
        printf("Un joueur ne peut posséder que 2 à 4 chevaux dans son écurie.\n");
        exit(0);
    }

    /* -----------------
       Le serveur écoute
       -----------------*/
    // Tant que tous les joueurs ne sont pas connectés
    int indice;
    
    for(indice = 0; indice < nbJoueursPartie; indice++)
    {
        id = fork();
        if (id == 0) // Fils
        {
            // Etablissement de la connexion
            int msgSock;
            msgSock = accept(num, NULL, NULL);
            // Actualisation du nombre de joueurs connectés
            nbJoueursConnectes++;
            
            // Message d'information: infos sur joueur
            char msg[50];
            sprintf(msg,"Vous êtes le joueur: %d\n", indice+1);
            write(msgSock, msg, strlen(msg));
            
            // Message d'information: places restantes
            int nbJoueursRestants;
            nbJoueursRestants = nbJoueursPartie - nbJoueursConnectes;
            printf("Un joueur s'est connecté, il reste %d place(s).\n", nbJoueursRestants);
            fflush(stdout);
            
        }
    }
    
    while (nbJoueursConnectes < nbJoueursPartie);
    
    printf("Tous les joueurs ont rejoint la partie.\n");

    return 0;
}

