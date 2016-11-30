# Makefile pour le projet de jeu des petits chevaux en ligne
# TODO : à compléter

CC = gcc --std=c99 -Wall -g -c
LD = gcc

PROG=testPlateau client serveur clientFermeTest
all: $(PROG)

clean:
	rm -f *~ *.o $(PROG)

client: Client.o Reseau.o Plateau.o GestionJeu.o
	$(LD) -o client Client.o Reseau.o Plateau.o GestionJeu.o
	
serveur: Serveur.o Reseau.h Serveur.h 
	$(LD) -o serveur Serveur.o Reseau.o

testPlateau: testPlateau.o Plateau.o GestionJeu.o
	$(LD) -o testPlateau testPlateau.o Plateau.o GestionJeu.o

GestionJeu.o: GestionJeu.c GestionJeu.h
	$(CC) GestionJeu.c

Plateau.o: Plateau.c Plateau.h GestionJeu.h
	$(CC) Plateau.c

Client.o: Client.c Reseau.h
	$(CC) Client.c

Reseau.o: Reseau.c Reseau.h
	$(CC) Reseau.c

Serveur.o: Serveur.c Serveur.h 
	$(CC) Serveur.c 
	
	

clientFermeTest: ClientFermeTest.o Reseau.o Plateau.o GestionJeu.o
	$(LD) -o clientFermeTest ClientFermeTest.o Reseau.o Plateau.o GestionJeu.o
	

ClientFermeTest.o: ClientFermeTest.c Reseau.h
	$(CC) ClientFermeTest.c
