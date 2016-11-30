#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "Reseau.h"
#include "Structure.h"
#include "Plateau.h"
#include "GestionJeu.h"

#define TAILLE 50

void recupDonnees(char* tabDonnees, joueur* tabReponse, int nbJoueurs, int nbChevaux);

int main(int argc, char **argv) {
    /* ----------------------------
   Initialisation des variables
   ----------------------------*/
    // Paramètres du programme
    //    int port;
    // 
    // int msgSock;
    // char buffer[TAILLE];
    int nbJoueurs, nbChevaux;

    int index;

    // int num;

    char* ptTableau[3];

    nbJoueurs = 3;
    nbChevaux = 3;




    ptTableau[0] = malloc(100 * sizeof (char));
    ptTableau[1] = malloc(100 * sizeof (char));
    ptTableau[2] = malloc(100 * sizeof (char));

    ptTableau[0] = "R:3-23-0;J:10-5-78;V:36-12-6";
    ptTableau[1] = "R:3-27-0;J:10-8-78;V:42-12-6";
    ptTableau[2] = "R:3-27-1;J:11-5-78;V:36-0-6";


    /*   num = read(msgSock, buffer, TAILLE);
       buffer[num] = '\0';
       printf("%s\n", buffer);
       //com client serv
       num = read(msgSock, buffer, TAILLE);
       printf("%d\n", num);
       buffer[num] = '\0';*/


    joueur ptAffichage[nbJoueurs];
    /*
        if (strcmp(buffer, "Que la partie commence !\n") == 0) {
            printf("%s\n", buffer);
            for (index = 0; index < 3; index++) {
                recupDonnees(ptTableau[index], ptAffichage, nbJoueurs, nbChevaux);
            }

            affichePlateau(5, 24);

            // Pour l'exemple, si on a besoin de parcourir la liste des équipes ?
            Team t;
            printf("Plateau de Jeu :\n");
            // N'affiche rien car 'R' > 'B'
            for (t = ROUGE; t <= BLEUE; t++) {
                printf("\t%s\n", toString(t));
            }
        }
     */ printf("a\n");
    recupDonnees(ptTableau[0], ptAffichage, nbJoueurs, nbChevaux);

    for (index = 0; index < 3; index++) {
        printf("%d\n", ptAffichage[0].ptChevaux[index].position);
    }

    return 0;
}

void recupDonnees(char* tabDonnees, joueur* tabReponse, int nbJoueurs, int nbChevaux) {
    char* resJoueurs[nbJoueurs]; // nombre de joueurs
    char* resJoueur; // couleur ET coordonnees chevaux
    char* token;
    char var[100];
    strcpy(var, tabDonnees);
    //char* resChevaux[nbChevaux]; // nombre de chevaux
    int index, indice;
    printf("b\n");
    for (index = 0; index < nbJoueurs; index++) {
        if (index == 0) {
            printf("b1\n");
            token = strtok(var, ";");
            printf("b2\n");
            int l = strlen(token);
            resJoueurs[index] = malloc(sizeof (char)*l);
            strcpy(resJoueurs[index], token);
        } else {
            resJoueurs[index] = strtok(NULL, ";");
        }
    }
    printf("c\n");
    for (index = 0; index < nbJoueurs; index++) {
        char* ptCouleur;
        ptCouleur = strtok(resJoueurs[index], ":");
        switch (ptCouleur[0]) {
            case 'R':
                sprintf(tabReponse[index].couleur, "%s", "Rouge");
                break;
            case 'J':
                sprintf(tabReponse[index].couleur, "%s", "Jaune");
                break;
            case 'V':
                sprintf(tabReponse[index].couleur, "%s", "Vert");
                break;
            case 'B':
                sprintf(tabReponse[index].couleur, "%s", "Bleu");
                break;
        }
        printf("d\n");
        token = strtok(NULL, ":");
        int l = strlen(token);
        resJoueur = malloc(sizeof (char)*l);
        strcpy(resJoueur, token);
        
        tabReponse[index].ptChevaux = malloc(nbChevaux*sizeof(cheval));
        
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
    printf("e\n");




}
