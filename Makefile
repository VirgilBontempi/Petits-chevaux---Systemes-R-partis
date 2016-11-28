# Makefile pour le projet de jeu des petits chevaux en ligne
# TODO : à compléter

CC = gcc --std=c99 -Wall -g -c
LD = gcc

PROG=testPlateau client
all: $(PROG)

clean:
	rm -f *~ *.o $(PROG)

client: client.o Reseau.o
	$(LD) -o client client.o Reseau.o

testPlateau: testPlateau.o Plateau.o GestionJeu.o
	$(LD) -o testPlateau testPlateau.o Plateau.o GestionJeu.o

GestionJeu.o: GestionJeu.c GestionJeu.h
	$(CC) GestionJeu.c

Plateau.o: Plateau.c Plateau.h GestionJeu.h
	$(CC) Plateau.c

client.o: Client.c Reseau.h
	$(CC) Client.c

Reseau.o: Reseau.c Reseau.h
	$(CC) Reseau.c
