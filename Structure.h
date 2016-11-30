/* 
 * File:   structure.h
 * Author: bontempv
 *
 * Created on 30 novembre 2016, 16:06
 */
#include <stdbool.h>

#define TAILLE_COULEUR 6

#ifndef STRUCTURE_H
#define	STRUCTURE_H

typedef struct {
    bool estArrive, estDansEnclos, estDansEscalier, estDansParcours;
    int position;
} cheval;

typedef struct {
    char couleur[TAILLE_COULEUR];
    cheval* ptChevaux;
} joueur;

typedef struct {
    int pipeIn[2], pipeOut[2];
    int numSock;
    joueur ptJoueur;
} structComCliServ;



#endif	/* STRUCTURE_H */

