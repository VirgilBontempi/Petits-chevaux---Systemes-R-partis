#include <stdbool.h>


#define TAILLE_MAX 100



void communicationProcessInit(int numSocket, int commSock, int nbJoueursPartie, bool* nbJoueursConnectes, structComCliServ* tab);
int comptePlacesRestantes(bool* tableauJoueurs, int nbJoueurs);
void InitJoueur(structComCliServ* tableauPipe, int nbJoueurs, int nbChevaux);
void ComProcess(structComCliServ* tab, int indice);
void attribueCouleur(int index, structComCliServ* tab);
