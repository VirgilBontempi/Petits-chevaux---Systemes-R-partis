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
#include <sys/wait.h>
#include <sys/socket.h>
#include "Reseau.h"

#define TAILLE 50

int main(int argc, char **argv)
{
	/* ----------------------------
       Initialisation des variables
       ----------------------------*/
	// Paramètres du programme
	int port;
    // 
	int msgSock;
	char buffer[TAILLE];
	
	port = atoi(argv[1]);
	
	// Connexion au serveur
	msgSock = socketClient("localhost", port, TCP);
	
	if(msgSock == -1)
	{ // Gestion d'erreur
		printf("Aie Aie Aie, des petits lutins ont volé la connexion !\n");
	}
	else
	{
		int num;
		
		//write(msgSock, "ping", 4);
		num = read(msgSock, buffer, TAILLE);
		buffer[num]='\0';
		printf("%s\n", buffer);
                //com client serv
                num=read(msgSock,buffer,TAILLE);
                printf("%d\n",num);
                buffer[num]='\0';
                //printf("%s\n",buffer);
                write(msgSock,"un trac",7);
                
                
	}
	
	return 0;
}

