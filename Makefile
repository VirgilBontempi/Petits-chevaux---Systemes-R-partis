# Makefile pour le projet de jeu des petits chevaux en ligne
# TODO : à compléter

CC = gcc --std=c99 -Wall -g -c
LD = gcc

PROG=testPlateau client
all: $(PROG)

clean:
	rm -f *~ *.o $(PROG)

client: Client.o Reseau.o
	$(LD) -o client Client.o Reseau.o
	
serveur: Serveur.o
	$(LD) -o serveur Serveur.o

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
