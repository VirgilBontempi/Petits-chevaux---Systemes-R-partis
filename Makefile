# Makefile pour le projet de jeu des petits chevaux en ligne
# TODO : à compléter

CC = gcc --std=c99 -Wall -g -c
LD = gcc

PROG=testPlateau client serveur clientFermeTest
all: $(PROG)

clean:
	rm -f *~ *.o $(PROG)

client: Client.o Reseau.o Plateau.o GestionJeu.o D.o
	$(LD) -o client Client.o Reseau.o Plateau.o GestionJeu.o D.o
	
serveur: Serveur.o Reseau.h Serveur.h GestionJeu.o D.o
	$(LD) -o serveur Serveur.o Reseau.o GestionJeu.o D.o

testPlateau: testPlateau.o Plateau.o GestionJeu.o
	$(LD) -o testPlateau testPlateau.o Plateau.o GestionJeu.o

GestionJeu.o: GestionJeu.c GestionJeu.h
	$(CC) GestionJeu.c

Plateau.o: Plateau.c Plateau.h GestionJeu.h
	$(CC) Plateau.c

Client.o: Client.c Reseau.h Client.h Plateau.h GestionJeu.h D.h
	$(CC) Client.c

Reseau.o: Reseau.c Reseau.h
	$(CC) Reseau.c

Serveur.o: Serveur.c Serveur.h GestionJeu.h D.h
	$(CC) Serveur.c 

D.o: D.c D.h
	$(CC) D.c



clientFermeTest: ClientFermeTest.o Reseau.o Plateau.o GestionJeu.o
	$(LD) -o clientFermeTest ClientFermeTest.o Reseau.o Plateau.o GestionJeu.o
	

ClientFermeTest.o: ClientFermeTest.c Reseau.h Plateau.h GestionJeu.h
	$(CC) ClientFermeTest.c
