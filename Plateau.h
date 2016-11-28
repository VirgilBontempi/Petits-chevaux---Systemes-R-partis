/* ---------------------------------------------------------------------------
 * Module de gestion et affichage du plateau de jeu
 * Auteur     : Damien Genthial
 */

#ifndef PLATEAU_H
#define PLATEAU_H

#ifndef PLATEAU_C
#define PUBLIC extern
#else
#define PUBLIC
#endif

// Les origines permettent de décaler un peu le plateau dans la fenêtre
PUBLIC void affichePlateau(int originRow, int originColumn);

#undef PUBLIC
#endif	/* PLATEAU_H */
