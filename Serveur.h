#define TAILLE_COULEUR 6
#define TAILLE_MAX 100


typedef struct {
	int pipeIn[2], pipeOut[2];
        int numSock;
	char couleur[TAILLE_COULEUR];} structComCliServ;




void communicationProcessInit(int numSocket, int commSock, int nbJoueursPartie, bool* nbJoueursConnectes, structComCliServ* tab);
int comptePlacesRestantes(bool* tableauJoueurs, int nbJoueurs);
void InitPipe(structComCliServ* tableauPipe, int nbJoueurs);
void ComProcess(structComCliServ* tab, int indice);
void attribueCouleur(int index, structComCliServ* tab);
