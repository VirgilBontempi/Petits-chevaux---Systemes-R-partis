#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "GestionJeu.h"

bool estDansLaLimite(int valDe, Team t, cheval Cheval)
{
	switch(t){
			case 'R' :
			 //on ne peut pas dépasser la case 56
				if(Cheval.position + valDe > 56)
					return false;
				break;
			case 'V' :
				//on ne peut pas dépasser la case 29
				if(Cheval.position + valDe > 27)
					return false;
				break;
			case 'B' :
				//on ne peut pas dépasser la case 43
				if(Cheval.position + valDe > 27)
					return false;
				break;
			case 'J' :
				//on ne peut pas dépasser la case 15
				if(Cheval.position + valDe > 14)
					return false;
				break;
			default:
			return true;
	}
	return false;	
}

bool caseCibleVide(int valDe, Team t, cheval c){
	
	//Quelle sera la case cible d'un cheval en avançant de n case
	char valWhichChar;
	valWhichChar = whichChar(c.position+valDe % 56);
	if(estDansLaLimite(valDe, t, c)){
		if (valWhichChar == 'R' || 'V' || 'B' || 'J' ){
			if (valWhichChar != t)
			{
				return true;
			}
		else
		{
			return false;
		}
	}
	else return false;
}

bool verifierVictoire(joueur j){
	int i;
	
	// Pour chaque cheval
	for(i=0; i<j.nbChevaux; i++){
		if((j.ptChevaux[i].estArrive) == false){
			// Le cheval courant n'est pas arrivé
			return false;
		}
	}
	// Tous les chevaux sont arrivés
	return true;
}

int nbChevauxEnclos(joueur j){
	int i, res;
	
	// Pour chaque cheval
	for(i=0; i<j.nbChevaux;i++){
		if(j.ptChevaux[i].estDansEnclos){
			res++; // augmentation nb chevaux dans enclos
		}
	}
	return res;
}

void sortirCheval(joueur j){
	int i;
	
	//Pour chaque cheval
	for(i=0; i<j.nbChevaux;i++){
		
		// Si un cheval est dans l'enclos
		if(j.ptChevaux[i].estDansEnclos){
			j.ptChevaux[i].position=1;          //on initialise sa position sur la position de départ
			j.ptChevaux[i].estDansEnclos=false; //le cheval n'est plus dans l'enclos
			break;
		}
	}	
}

int getPlaceCheval(cheval chev){
	
	return chev.position;
}

int getValeurCaseEscalier(int numCase, Team t){
	switch(t){
		case 'R' : 
			return numCase-=60;
			break;
		case 'J' :  
			return numCase-=70;
			break;
		case 'V' : 
			return numCase-=80;
			break;
		case 'B' : 
			return numCase-=90;
			break;
		default:
			return 0;
	}
	return 0;
}

/*bool caseVide(int numCase, Team t){
	char charCase[10];
	charCase[10] = whichChar(numCase);
	if ((strcmp(charCase, 'R') == 0) || (strcmp(charCase, 'J') == 0) || (strcmp(charCase, 'V') == 0) || (strcmp(charCase, 'B') == 0)){
		return false;
	}
	return true;
}*/

void entrerChevalEscalier(joueur j){
	int i;
	for(i=0; i<j.nbChevaux; i++){
		if(j.ptChevaux[i].position == 56){
			j.ptChevaux[i].estDansEscalier;
		}
	}
}

void avancerChevalEscalier(joueur j, int valeurDe, int numCase, Team t){
	
	int i;
	//pour chaque cheval
	for(i=0; i<j.nbChevaux;i++){
		
		// si un cheval est dans l'escalier
		if(j.ptChevaux[i].estDansEscalier){
			
			// si la valeur de la case ou le pion est egal à la valeur du dé -1
			if(getValeurCaseEscalier(numCase, t) == valeurDe-1){
				//on deplace le cheval sur sa nouvelle position
				j.ptChevaux[i].position+=1;
				
				//si la nouvelle position du pion est 6, on sort le cheval
				if(j.ptChevaux[i].position==6){
					j.ptChevaux[i].estArrive=true;
					j.ptChevaux[i].estDansEscalier=false;
				}
				
			}
			if(j.ptChevaux[i].position == 56){
			entrerChevalEscalier(j);
			}
		}
	}
}
