#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include "Reseau.h"
#include "GestionJeu.h"
#include "Serveur.h"
#include "D.h"

int main(int argc, char **argv) {
    // Initialisation des variables
    // Paramètres du programme
    int port, nbJoueursPartie, nbChevaux;
    int indice, ind;
    int num;


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


        // Etabli la connexion entre le serveur et un client
        communicationProcessInit(num, indice, nbJoueursPartie, tableauJoueursPrets, tableauJoueurs);

        // Un client vient de se connecter
        indice++;

    }

    // Tous les joueurs sont connectés
    printf("Tous les joueurs ont rejoint la partie.\n");

    // Pour chaque joueur
    for (ind = 0; ind < nbJoueursPartie; ind++) {
        // Variable
        pid_t id;
        char buffer[TAILLE_MAX];
        //fermeture des pipe 
        //fermePipe(tableauJoueurs, ind);

        id = fork();
        if (id == 0) // Fils
        {
            ComProcess(tableauJoueurs, ind);
            exit(0);

        } else { // Père


            // Fermeture des parties des tubes que l'on utilise pas
            close(tableauJoueurs[ind].pipeIn[0]);
            close(tableauJoueurs[ind].pipeOut[1]);

            sleep(2);
            // Envoie du nombre de chevaux
            sprintf(buffer, "%d", nbChevaux);
            write(tableauJoueurs[ind].pipeIn[1], buffer, strlen(buffer));

            sleep(2);

            // Ecriture du message de lancement de partie
            sprintf(buffer, "Que la partie commence !\n");
            write(tableauJoueurs[ind].pipeIn[1], buffer, strlen(buffer));

            sleep(2);

            // Construction de la chaîne d'initialisation de la partie
            // Et envoie du nombre de chevaux
            construitChaineEtatJeu(tableauJoueurs, nbChevaux, 4, buffer);
            write(tableauJoueurs[ind].pipeIn[1], buffer, strlen(buffer));

        }
    }
    
    
    
    //En cour de construction
    
  /*  char buffer[TAILLE_MAX];
    int taille, numDe;
    while (true/*Partie non terminer*//*) {
    /*    for (ind = 0; ind < nbJoueursPartie; ind++) {

            sprintf(buffer, "A ton tour !\n");
            write(tableauJoueurs[ind].pipeIn[1], buffer, strlen(buffer));
            //attend le retour du jet de dé
            taille = read(tableauJoueurs[ind].pipeOut[0], buffer, TAILLE_MAX);
            numDe = atoi(buffer);
            afficheResDe(numDe);
            
            

            // calcul proposition
            //envoi des proposition
            //reception du choix
            //application du choix
            //renvoi du plateau

            //regarde si victoire
            //si oui breack
            //sinon continu de tourner

        }
    }*/
    return 0;
}

void construitChaineEtatJeu(structComCliServ* tableauJoueurs, int nbChevaux, int nbJoueurs, char chaine[]) {
    // Variables
    int indice, index;
    chaine[0] = '\0';
    // Pour tous les joueurs
    for (indice = 0; indice < nbJoueurs; indice++) {
        char couleur[16];
        sprintf(couleur, "%c:", tableauJoueurs[indice].ptJoueur.couleur);
        strcat(chaine, couleur);
        // Pour tous les chevaux
        for (index = 0; index < nbChevaux; index++) {
            // Si c'est le dernier cheval
            if (index == nbChevaux - 1) {
                // Concaténation de la position du cheval courant et de ;
                char buffer[16];
                sprintf(buffer, "%02d;", tableauJoueurs[indice].ptJoueur.ptChevaux[index].position);
                strcat(chaine, buffer);
            } else {
                // Concaténation de la position du cheval courant et de -
                char buffer[16];
                sprintf(buffer, "%02d-", tableauJoueurs[indice].ptJoueur.ptChevaux[index].position);
                strcat(chaine, buffer);
            }
        }
    }
    // chaine = R:0-0-0-0;J:0-0-0-0;V:0-0-0-0;B:0-0-0-0;
    // Le nombre de 0 dépend du nombre de cheval
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
    char msg[TAILLE_MAX];
    //attribueCouleur(index, tab);
    sprintf(msg, "Connexion établie.\nVous êtes le joueur: %s\n\nEn attente de %d joueurs...", toString(tab[index].ptJoueur.couleur), comptePlacesRestantes(tableauJoueurs, nbJoueursPartie));
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
    char msgRequestServ[TAILLE_MAX];
    char msgReply[TAILLE_MAX];
    int taille;


    // Fermeture des parties des tubes que l'on utilise pas
    // fermePipe(tab, indice);
    close(tab[indice].pipeIn[1]);
    close(tab[indice].pipeOut[0]);

    // Lecture dans le tube
    taille = read(tab[indice].pipeIn[0], msgRequestServ, TAILLE_MAX);
    // Ecriture dans la socket (nbChevaux)
    write(tab[indice].numSock, msgRequestServ, taille);

    sleep(1);

    // Lecture dans le tube
    taille = read(tab[indice].pipeIn[0], msgRequestServ, TAILLE_MAX);
    // Ecriture dans la socket (Que la partie commence !)
    write(tab[indice].numSock, msgRequestServ, taille);

    sleep(1);

    // Lecture dans le tube
    taille = read(tab[indice].pipeIn[0], msgRequestServ, TAILLE_MAX);
    // Ecriture dans la socket (Plateau à l'état d'origine)
    write(tab[indice].numSock, msgRequestServ, taille);

    
    //En cour de construction
    
   /* while (true/*Partie non fini*//*) {                                       
        taille = read(tab[indice].pipeIn[0], msgRequestServ, TAILLE_MAX);
        // Ecriture dans la socket (Plateau à l'état d'origine)
        write(tab[indice].numSock, msgRequestServ, taille);
        // Lecture dans la socket (le client parle)
        taille = read(tab[indice].numSock, msgReply, TAILLE_MAX);
        msgReply[taille] = '\0';
        // Ecriture dans le tube (réponse)
        write(tab[indice].pipeOut[1], msgReply, taille + 1);
    }*/






    //fermeture de la socket
    close(tab[indice].numSock);

}

/* ------------------------------
 * Fermeture des pipes inutilisés
 * ------------------------------*/
void fermePipe(structComCliServ* joueurs, int ind) {
    int index;
    for (index = 0; index < 4; index++) {
        if (index != ind) {
            close(joueurs[index].pipeIn[0]);
            close(joueurs[index].pipeIn[1]);
            close(joueurs[index].pipeOut[0]);
            close(joueurs[index].pipeOut[1]);
        }
    }
}