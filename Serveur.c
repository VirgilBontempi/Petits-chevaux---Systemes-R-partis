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

int main(int argc, char **argv) {
    /* ----------------------------
     * Initialisation des variables
     * ----------------------------*/
    // Paramètres du programme
    int port, nbJoueursPartie, nbChevaux;
    int indice, ind;
    int num;

    port = atoi(argv[1]);
    nbJoueursPartie = atoi(argv[2]);
    nbChevaux = atoi(argv[3]);

    // Tableau de structure des processus, taille = nbJoueursPartie
    structComCliServ tableauPipe[nbJoueursPartie];

    // Nombre de joueurs connectés
    bool tableauJoueurs[nbJoueursPartie];

    //initialisation des pipe Process
    InitPipe(tableauPipe, nbJoueursPartie);

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
    printf("La partie est ouverte.\nEn attente de %d joueur(s)...\n", nbJoueursPartie);

    // Tant que tous les joueurs ne sont pas connectés

    indice = 0;

    while (indice < nbJoueursPartie) {

        pid_t id;
        id = fork();
        if (id == 0) // Fils
        {
            communicationProcessInit(num, indice, nbJoueursPartie, tableauJoueurs, tableauPipe);

        } else {
            while (tableauJoueurs[indice] == false);
        }
        indice++;

    }
    printf("OHE\n");
    // Tant que tous les joueurs ne sont pas connectés
    while (comptePlacesRestantes(tableauJoueurs, nbJoueursPartie) != 0);

    // Tous les joueurs sont connectés
    printf("Tous les joueurs ont rejoint la partie.\n");

    int i=0;

    for (ind = 0; ind < nbJoueursPartie; ind++) {
        pid_t id;
        int taille;
        char reponse[50];
        id = fork();
        if (id == 0) // Fils
        {
            ComProcess(tableauPipe, ind);
            exit(0);

        } else {
            close(tableauPipe[ind].pipeIn[0]);
            close(tableauPipe[ind].pipeOut[1]);
            write(tableauPipe[ind].pipeIn[1], "entre un truc", 13);
            printf("la\n");
            taille = read(tableauPipe[ind].pipeOut[0], reponse, 50);
            reponse[taille] = '\0';
            printf("%s\n", reponse);
            
        }
    }
    
    return 0;
}

/* --------------------------
 * Processus de communication
 * --------------------------*/
void communicationProcessInit(int numSocket, int index, int nbJoueursPartie, bool* tableauJoueurs, structComCliServ* tab) {

    // Etablissement de la connexion
    int msgSock;
    msgSock = accept(numSocket, NULL, NULL);
    // Actualisation du nombre de joueurs connectés
    tableauJoueurs[index] = true;
    tab[index].numSock = msgSock;

    // Message d'information: infos sur joueur
    char msg[50];
    sprintf(msg, "Connexion établie.\nVous êtes le joueur: %d\n", index + 1);
    write(msgSock, msg, strlen(msg));

    // Message d'information: places restantes
    printf("Un joueur s'est connecté, il reste %d place(s).\n", comptePlacesRestantes(tableauJoueurs, nbJoueursPartie));
    fflush(stdout);



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

void InitPipe(structComCliServ* tableauPipe, int nbJoueurs) {
    int indice;
    for (indice = 0; indice < nbJoueurs; indice++) {

        pipe(tableauPipe[indice].pipeIn);
        pipe(tableauPipe[indice].pipeOut);

    }
}

void ComProcess(structComCliServ* tab, int indice) {
    char msgRequest[TAILLE_MAX];
    char msgReply[TAILLE_MAX];
    int taille;

    close(tab[indice].pipeIn[1]);
    close(tab[indice].pipeOut[0]);
    taille = read(tab[indice].pipeIn[0], msgRequest, TAILLE_MAX);
    msgRequest[taille] = '\0';
    printf("comm : taille alle :%d\n", taille);
    write(tab[indice].numSock, msgRequest, taille + 1);
    taille = read(tab[indice].numSock, msgReply, TAILLE_MAX);
    msgReply[taille] = '\0';
    printf("comm : taille retour :%d\n", taille);
    write(tab[indice].pipeOut[1], msgReply, taille + 1);
}
