#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "Reseau.h"
#include "Plateau.h"
#include "GestionJeu.h"

#define TAILLE 50

void recupDonnees(char* tabDonnees, joueur* tabReponse, int nbChevaux);

int main(int argc, char **argv) {
    /* ----------------------------
   Initialisation des variables
   ----------------------------*/
    // Paramètres du programme
    //    int port;
    // 
    // int msgSock;
    char buffer[TAILLE];
    int nbChevaux;

    int index, indice;

    char* ptTableau[3];

    nbChevaux = 3;




    ptTableau[0] = malloc(100 * sizeof (char));
    ptTableau[1] = malloc(100 * sizeof (char));
    ptTableau[2] = malloc(100 * sizeof (char));

    ptTableau[0] = "R:3-23-0;J:10-5-72;V:36-12-6;B:0-0-0";
    ptTableau[1] = "R:3-27-0;J:10-8-72;V:42-12-6;B:0-0-0";
    ptTableau[2] = "R:3-27-1;J:11-5-72;V:36-0-6;B:0-0-0";


    /*   num = read(msgSock, buffer, TAILLE);
       buffer[num] = '\0';
       printf("%s\n", buffer);
       //com client serv
       num = read(msgSock, buffer, TAILLE);
       printf("%d\n", num);
       buffer[num] = '\0';*/



    sprintf(buffer, "%s", "Que la partie commence !\n");
    if (strcmp(buffer, "Que la partie commence !\n") == 0) {
        printf("%s\n", buffer);
        for (index = 0; index < 3; index++) {
            recupDonnees(ptTableau[index], TabJoueurs, nbChevaux);
            affichePlateau(5, 24);
        }


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
                printf("%d\n", TabJoueurs[index].ptChevaux[indice].position);
            }
        }
    }

    //recupDonnees(ptTableau[0], ptAffichage, nbJoueurs, nbChevaux);



    return 0;
}

void recupDonnees(char* tabDonnees, joueur* tabReponse, int nbChevaux) {
    char* resJoueurs[NB_JOUEURS]; // nombre de joueurs
    char* resJoueur; // couleur ET coordonnees chevaux
    char* token;
    char var[100];
    strcpy(var, tabDonnees);
    //char* resChevaux[nbChevaux]; // nombre de chevaux
    int index, indice;
    for (index = 0; index < NB_JOUEURS; index++) {
        if (index == 0) {
            token = strtok(var, ";");
            int l = strlen(token);
            resJoueurs[index] = malloc(sizeof (char)*l);
            strcpy(resJoueurs[index], token);
        } else {
            resJoueurs[index] = strtok(NULL, ";");
        }
    }
    for (index = 0; index < NB_JOUEURS; index++) {
        char* ptCouleur;
        ptCouleur = strtok(resJoueurs[index], ":");
        switch (ptCouleur[0]) {
            case 'R':
                tabReponse[index].couleur = ROUGE;
                break;
            case 'J':
                tabReponse[index].couleur = JAUNE;
                break;
            case 'V':
                tabReponse[index].couleur = VERTE;
                break;
            case 'B':
                tabReponse[index].couleur = BLEUE;
                break;
        }
        token = strtok(NULL, ":");
        int l = strlen(token);
        resJoueur = malloc(sizeof (char)*l);
        strcpy(resJoueur, token);

        tabReponse[index].nbChevaux = nbChevaux;
        tabReponse[index].ptChevaux = malloc(nbChevaux * sizeof (cheval));

        for (indice = 0; indice < nbChevaux; indice++) {

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
