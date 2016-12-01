/* 
 * File:   Client.h
 * Author: matheyt
 *
 * Created on 1 décembre 2016, 10:34
 */

#ifndef CLIENT_H
#define	CLIENT_H

#ifdef	__cplusplus
extern "C" {
#endif

#define TAILLE 100

void afficherTour(char* buffer, int nbChevaux);
void recupDonnees(char* tabDonnees, joueur* tabReponse, int nbChevaux);

#ifdef	__cplusplus
}
#endif

#endif	/* CLIENT_H */

