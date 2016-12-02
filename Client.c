/*
 * Client.c
 * 
 * Copyright 2016 Virgil Bontempi <bontempv@vercors11>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "Reseau.h"
#include "Plateau.h"
#include "GestionJeu.h"
#include "Client.h"
#include "D.h"

int main(int argc, char **argv) {
    // Variables
    int port, msgSock;
    char buffer[TAILLE];

    int nbChevaux;

    port = atoi(argv[2]);

    // Connexion au serveur
    msgSock = socketClient(argv[1], port, TCP);

    if (msgSock == -1) { // Gestion d'erreur
        printf("Aie Aie Aie, des petits lutins ont volé la connexion !\n");
    } else {
        int num;

        // Connexion etablie, vous etes le joueur ...
        num = read(msgSock, buffer, TAILLE);
        buffer[num] = '\0';
        printf("%s\n", buffer);


        // Réception du nombre de chevaux
        memset(buffer, '\0', TAILLE);
        num = read(msgSock, buffer, TAILLE);
        nbChevaux = atoi(buffer);

        // On attend "Que la partie commence" (le feu vert)
        memset(buffer, '\0', TAILLE);
        num = read(msgSock, buffer, TAILLE);
        printf("%s\n", buffer);

        // Lancement de la partie
        if (strcmp(buffer, "Que la partie commence !\n") == 0) {
            // On a reçu le feu vert
            num = read(msgSock, buffer, TAILLE);
            // Affichage du plateau à l'état d'origine
            afficherTour(buffer, nbChevaux);
            
            
            //En cour de construction
            
            
/*            while (strcmp(buffer, "STOP") != 0) {
                // Lancement de dé
                num=read(msgSock,buffer,TAILLE);
                while(strcmp(buffer,"A ton tour !\n")!=0)
                {
                    num=read(msgSock,buffer,TAILLE);
                }*/
                printf("\nLe dé roule...\n\n");
                int valeurDe;
                valeurDe = lancerDe();
                afficheResDe(valeurDe);
                /*sprintf(buffer,"%d",valeurDe);
                write(msgSock,buffer,strlen(buffer));
            }*/
        }
    }

    //fermeture de la socket
    close(msgSock);
    return 0;
}

/* --------------------------------------------
 * Affiche le plateau à la fin d'un tour de jeu
 * --------------------------------------------*/
void afficherTour(char* buffer, int nbChevaux) {
    // Variables
    int index, indice;

    // Actualise les données suit à un tour de jeu
    recupDonnees(buffer, TabJoueurs, nbChevaux);
    // Affiche le plateau
    affichePlateau(5, 24);


    // Pour l'exemple, si on a besoin de parcourir la liste des équipes ?
    Team t;
    printf("Plateau de Jeu :\n");
    // N'affiche rien car 'R' > 'B'
    for (t = ROUGE; t <= BLEUE; t++) {
        printf("\t%s\n", toString(t));
    }
    for (index = 0; index < NB_JOUEURS; index++) {
        printf("%s :\n", toString(TabJoueurs[index].couleur));
        for (indice = 0; indice < nbChevaux; indice++) {
            printf("Cheval %d : %d\n", indice, TabJoueurs[index].ptChevaux[indice].position);
        }
    }
}

/* -------------------------------------
 * Récupère les données de l'état du jeu
 * -------------------------------------*/
void recupDonnees(char* tabDonnees, joueur* tabReponse, int nbChevaux) {
    // Variables
    // Nombre de joueurs
    char* resJoueurs[NB_JOUEURS];
    // couleur ET coordonnees chevaux
    char* resJoueur;
    char* token;
    char var[100];
    int index, indice;

    strcpy(var, tabDonnees);
    // Pour chaque joueur
    for (index = 0; index < NB_JOUEURS; index++) {
        // Récupération des données d'un joueur (couleur et chevaux)
        if (index == 0) {
            token = strtok(var, ";");
            int l = strlen(token);
            resJoueurs[index] = malloc(sizeof (char)*l);
            strcpy(resJoueurs[index], token);
        } else {
            // On reprend la où on en était
            resJoueurs[index] = strtok(NULL, ";");
        }
    }

    // Pour chaque joueurs
    for (index = 0; index < NB_JOUEURS; index++) {
        // Affectation des données d'un joueur (couleur)
        char* ptCouleur;
        ptCouleur = strtok(resJoueurs[index], ":");

        switch (ptCouleur[0]) {
            case 'R': // Le joueur est rouge
                tabReponse[index].couleur = ROUGE;
                break;
            case 'J': // Le joueur est jaune
                tabReponse[index].couleur = JAUNE;
                break;
            case 'V': // Le joueur est vert
                tabReponse[index].couleur = VERTE;
                break;
            case 'B': // Le joueur est bleu
                tabReponse[index].couleur = BLEUE;
                break;
        }

        // On reprend la où on en était
        token = strtok(NULL, ":");
        int l = strlen(token);
        fflush(stdout);
        resJoueur = malloc(sizeof (char)*l);

        strcpy(resJoueur, token);
        // Affectation des données d'un joueur (nombre de chevaux)
        tabReponse[index].nbChevaux = nbChevaux;
        tabReponse[index].ptChevaux = malloc(nbChevaux * sizeof (cheval));
        // Pour chaqaue cheval
        for (indice = 0; indice < nbChevaux; indice++) {
            // Récupération des données d'un cheval (position)
            if (indice == 0) {
                char* ptPosi;
                token = strtok(resJoueur, "-");
                int l = strlen(token);
                ptPosi = malloc(sizeof (char)*l);
                strcpy(ptPosi, token);
                tabReponse[index].ptChevaux[indice].position = atoi(ptPosi);
            } else {
                char* ptPosi;
                token = strtok(NULL, "-");
                int l = strlen(token);
                ptPosi = malloc(sizeof (char)*l);
                strcpy(ptPosi, token);
                tabReponse[index].ptChevaux[indice].position = atoi(ptPosi);
            }
        }
    }
}


