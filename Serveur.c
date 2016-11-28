/*
 * Serveur.c
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
#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "Serveur.h"

int main(int argc, char **argv)
{
	/* ----------------------------
	   Initialisation des variables
	   ----------------------------*/
	// Paramètres du programme
	int port, nbJoueursPartie, nbChevaux;
	// Nombre de joueurs connectés
	int nbJoueursConnectes;
	// Tableau de structure des processus, taille = nbJoueursPartie
	char tableauPipe[nbJoueursPartie];
	
	port = atoi(argv[1]);
	nbJoueursPartie = atoi(argv[2]);
	nbChevaux = atoi(argv[3]);
	
	nbJoueursConnectes = 0;
	
	num = socketServer(port, TCP);
	
	// Saisie controlée nb joueurs de la partie
	if(nbJoueursPartie > 4 || nbJoueursPartie < 2)
	{
		printf("Une partie doit contenir entre 2 et 4 joueurs.");
		exit(0);
	}
	// Saisie controlée nb chevaux de la partie
	if(nbChevaux > 4 || nbChevaux < 2)
	{
		printf("Un joueur ne peut posséder que 2 à 4 chevaux dans son écurie.");
		exit(0);
	}
	
	/* -----------------
	   Le serveur écoute
	   -----------------*/
	int indice;
	// Tant que tous les joueurs ne sont pas connectés
	for(indice = 0; indice < nbJoueursConnectes; indice++)
	{
		id = fork();
		if(id == 0) // Fils
		{
			int msgSock;
			msgSock = accept(num, NULL, NULL);
		}
		
	}
	
	
	
	return 0;
}

