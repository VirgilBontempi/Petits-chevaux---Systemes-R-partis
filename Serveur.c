#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>
#include "Reseau.h"
#include "GestionJeu.h"
#include "Serveur.h"

int main(int argc, char **argv) {
    // Initialisation des variables
    // Paramètres du programme
    int port, nbJoueursPartie, nbChevaux;
    int indice, ind;
    int num;
    char buffer[TAILLE_MAX];

    port = atoi(argv[1]);
    nbJoueursPartie = atoi(argv[2]);
    nbChevaux = atoi(argv[3]);

    // Tableau de structure des processus, taille = nbJoueursPartie
    structComCliServ tableauJoueurs[4];

    // Nombre de joueurs connectés
    bool tableauJoueursPrets[nbJoueursPartie];

    //initialisation des pipe Process
    InitJoueur(tableauJoueurs, 4, nbChevaux);

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

    // Le serveur écoute
    printf("La partie est ouverte.\nEn attente de %d joueur(s)...\n", nbJoueursPartie);

    // Tant que tous les joueurs ne sont pas connectés
    indice = 0;
    while (indice < nbJoueursPartie) {
        pid_t id;
        id = fork();
        if (id == 0) // Fils
        {
            // Etabli la connexion entre le serveur et un client
            communicationProcessInit(num, indice, nbJoueursPartie, tableauJoueursPrets, tableauJoueurs);
        } else { // Père
            // Le père attends la fin de l'execution du fils
            wait(NULL);
        }

        // Un client vient de se connecter
        indice++;

    }

    // Tant que tous les joueurs ne sont pas connectés
    while (comptePlacesRestantes(tableauJoueursPrets, nbJoueursPartie) != 0);

    // Tous les joueurs sont connectés
    printf("Tous les joueurs ont rejoint la partie.\n");

    // Pour chaque joueur
    for (ind = 0; ind < nbJoueursPartie; ind++) {
        // Variable
        pid_t id;

        id = fork();
        if (id == 0) // Fils
        {
            ComProcess(tableauJoueurs, ind);
            exit(0);

        } else { // Père
            // Fermeture des parties des tubes que l'on utilise pas
            close(tableauJoueurs[ind].pipeIn[0]);
            close(tableauJoueurs[ind].pipeOut[1]);

            // Envoie du nombre de chevaux
            sprintf(buffer, "%d", nbChevaux);
            write(tableauJoueurs[ind].pipeIn[1], buffer, 4);

            // Ecriture du message de lancement de partie
            write(tableauJoueurs[ind].pipeIn[1], "Que la partie commence !\n", 25);

            // Construction de la chaîne d'initialisation de la partie
            // Et envoie du nombre de chevaux
            sprintf(buffer, "%s", construitChaineEtatJeu(tableauJoueurs, nbChevaux, 4));
            write(tableauJoueurs[ind].pipeIn[1], buffer, TAILLE_MAX);
        }
    }
    return 0;
}

char* construitChaineEtatJeu(structComCliServ* tableauJoueurs, int nbChevaux, int nbJoueurs) {
    // Variables
    char* chaine;
    int indice, index;

    chaine = malloc(TAILLE_MAX * sizeof (char));

    // Pour tous les joueurs
    for (indice = 0; indice < nbJoueurs; indice++) {
        strcat(chaine, tableauJoueurs[indice].ptJoueur.couleur + ":");

        // Pour tous les chevaux
        for (index = 0; index < nbChevaux; index++) {
            // Si c'est le dernier cheval
            if (index == nbChevaux - 1) {
                // Concaténation de la position du cheval courant et de ;
                strcat(chaine, tableauJoueurs[indice].ptJoueur.ptChevaux[index].position + ";");
            } else {
                // Concaténation de la position du cheval courant et de -
                strcat(chaine, tableauJoueurs[indice].ptJoueur.ptChevaux[index].position + "-");
            }
        }
    }

    // chaine = R:0-0-0-0;J:0-0-0-0;V:0-0-0-0;B:0-0-0-0;
    // Le nombre de 0 dépend du nombre de cheval
    return chaine;
}

/* ---------------------------------------
 * Créarion des processus de communication
 * ---------------------------------------*/
void communicationProcessInit(int numSocket, int index, int nbJoueursPartie, bool* tableauJoueurs, structComCliServ* tab) {

    // Etablissement de la connexion
    int msgSock;
    msgSock = accept(numSocket, NULL, NULL);
    // Actualisation du nombre de joueurs connectés
    tableauJoueurs[index] = true;
    tab[index].numSock = msgSock;

    // Message d'information: infos sur joueur
    char msg[50];
    //attribueCouleur(index, tab);
    sprintf(msg, "Connexion établie.\nVous êtes le joueur: %s\n", toString(tab[index].ptJoueur.couleur));
    write(msgSock, msg, strlen(msg));

    // Message d'information: places restantes
    printf("Le joueur %s s'est connecté, il reste %d place(s).\n", toString(tab[index].ptJoueur.couleur), comptePlacesRestantes(tableauJoueurs, nbJoueursPartie));
    fflush(stdout);



}

/* -------------------------------------
 * Attribution d'une couleur à un joueur
 * -------------------------------------*/
void attribueCouleur(int index, structComCliServ* tab) {
    switch (index) {
        case 0: // Permier client connecté
            tab[index].ptJoueur.couleur = ROUGE;
            break;
        case 1: // Deuxième client connecté
            tab[index].ptJoueur.couleur = JAUNE;
            break;
        case 2: // Troisième client connecté
            tab[index].ptJoueur.couleur = VERTE;
            break;
        case 3: // Quatrième client connecté
            tab[index].ptJoueur.couleur = BLEUE;
            break;
    }
}

/* -------------------------------------
 * Compte les nombre de places restantes
 * -------------------------------------*/
int comptePlacesRestantes(bool* tableauJoueurs, int nbJoueurs) {
    // Variables
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

/* --------------------
 * Initialise les pipes
 * --------------------*/
void InitJoueur(structComCliServ* tableau, int nbJoueurs, int nbChevaux) {
    // Variables
    int indice, index;

    // Pour chaque joueur
    for (indice = 0; indice < nbJoueurs; indice++) {
        // Initialisation
        pipe(tableau[indice].pipeIn);
        pipe(tableau[indice].pipeOut);

        // Initialisation du joueur
        joueur newJoueur;
        // Initialisation du tableau de chevaux du joueur
        newJoueur.ptChevaux = malloc(nbChevaux * sizeof (cheval));

        for (index = 0; index < nbChevaux; index++) {
            // Initialisation d'un cheval
            cheval newCheval;
            newCheval.estArrive = false;
            newCheval.estDansEnclos = true;
            newCheval.estDansEscalier = false;
            newCheval.estDansParcours = false;
            newCheval.position = 0;

            newJoueur.ptChevaux[index] = newCheval;
        }

        // Affectation du joueur
        tableau[indice].ptJoueur = newJoueur;

        // Attribution de la couleur
        attribueCouleur(indice, tableau);
    }
}

/* ------------------------
 * Process de communication
 * ------------------------*/
void ComProcess(structComCliServ* tab, int indice) {
    // Variables
    char msgRequest[TAILLE_MAX];
//    char msgReply[TAILLE_MAX];
    int taille;

    // Fermeture des parties des tubes que l'on utilise pas
    close(tab[indice].pipeIn[1]);
    close(tab[indice].pipeOut[0]);

    // Lecture dans le tube
    taille = read(tab[indice].pipeIn[0], msgRequest, TAILLE_MAX);
    msgRequest[taille] = '\0';
    // Ecriture dans la socket (nbChevaux)
    write(tab[indice].numSock, msgRequest, taille + 1);
    
    // Lecture dans le tube
    taille = read(tab[indice].pipeIn[0], msgRequest, TAILLE_MAX);
    msgRequest[taille] = '\0';
    // Ecriture dans la socket (Que la partie commence !)
    write(tab[indice].numSock, msgRequest, taille + 1);
    
    // Lecture dans le tube
    taille = read(tab[indice].pipeIn[0], msgRequest, TAILLE_MAX);
    msgRequest[taille] = '\0';
    // Ecriture dans la socket (Plateau à l'état d'origine)
    write(tab[indice].numSock, msgRequest, taille + 1);
    
    
    
    
    // Lecture dans la socket (le client parle)
    /*taille = read(tab[indice].numSock, msgReply, TAILLE_MAX);
    msgReply[taille] = '\0';
    // Ecriture dans le tube (réponse)
    write(tab[indice].pipeOut[1], msgReply, taille + 1);*/
}