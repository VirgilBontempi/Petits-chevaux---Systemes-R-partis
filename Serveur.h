#include <stdbool.h>

#define TAILLE_COULEUR 6
#define TAILLE_MAX 100

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



void communicationProcessInit(int numSocket, int commSock, int nbJoueursPartie, bool* nbJoueursConnectes, structComCliServ* tab);
int comptePlacesRestantes(bool* tableauJoueurs, int nbJoueurs);
void InitJoueur(structComCliServ* tableauPipe, int nbJoueurs, int nbChevaux);
void ComProcess(structComCliServ* tab, int indice);
void attribueCouleur(int index, structComCliServ* tab);
