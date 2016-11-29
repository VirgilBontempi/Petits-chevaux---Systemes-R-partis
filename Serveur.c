#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>
#include "Reseau.h"
#include "Serveur.h"

void communicationProcess(int numSocket, int commSock, int nbJoueursPartie, bool* nbJoueursConnectes);
int comptePlacesRestantes(bool* tableauJoueurs, int nbJoueurs);

int main(int argc, char **argv) {
    /* ----------------------------
     * Initialisation des variables
     * ----------------------------*/
    // Paramètres du programme
    int port, nbJoueursPartie, nbChevaux;

    int num;

    port = atoi(argv[1]);
    nbJoueursPartie = atoi(argv[2]);
    nbChevaux = atoi(argv[3]);

    // Tableau de structure des processus, taille = nbJoueursPartie
    char tableauPipe[nbJoueursPartie];

    // Nombre de joueurs connectés
    bool tableauJoueurs[nbJoueursPartie];

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
     * Le serveur écoute
     * -----------------*/
    printf("La partie est ouverte.\nEn attente de joueurs %d...\n", nbJoueursPartie);

    // Tant que tous les joueurs ne sont pas connectés
    int indice;

    for (indice = 0; indice < nbJoueursPartie; indice++) {
        communicationProcess(num, indice, nbJoueursPartie, tableauJoueurs);
    }

    // Tant que tous les joueurs ne sont pas connectés
    while (comptePlacesRestantes(tableauJoueurs, nbJoueursPartie) != 0);
    // Tous les joueurs sont connectés
    printf("Tous les joueurs ont rejoint la partie.\n");

    return 0;
}

/* --------------------------
 * Processus de communication
 * --------------------------*/
void communicationProcess(int numSocket, int commSock, int nbJoueursPartie, bool* tableauJoueurs) {
    pid_t id;
    id = fork();
    
    if (id == 0) // Fils
    {
        // Etablissement de la connexion
        int msgSock;
        msgSock = accept(numSocket, NULL, NULL);
        // Actualisation du nombre de joueurs connectés
        tableauJoueurs[commSock] = true;

        // Message d'information: infos sur joueur
        char msg[50];
        sprintf(msg, "Connexion établie.\nVous êtes le joueur: %d\n", commSock + 1);
        write(msgSock, msg, strlen(msg));

        // Message d'information: places restantes
        printf("Un joueur s'est connecté, il reste %d place(s).\n", comptePlacesRestantes(tableauJoueurs, nbJoueursPartie));
        fflush(stdout);
    } else {
        // Attend que le fils se termine
        wait(NULL);
    }
}

/* -------------------------------------
 * Compte les nombre de places restantes
 * -------------------------------------*/
int comptePlacesRestantes(bool* tableauJoueurs, int nbJoueurs) {
    int index;
    int nbRestant;

    nbRestant = 0;
    
    // Pour chaque joueur du tableau de joueurs
    for (index = 0; index < nbJoueurs; index++) {
        // Si le joueur courant n'est pas pret
        if (tableauJoueurs[index] == false) {
            // Incrémentation du nombre de places restantes
            nbRestant++;
        }
    }
    return nbRestant;
}
