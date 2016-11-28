/* Module pour l'ouverture de connexions UDP ou TCP client ou serveur
 * Auteur : Damien Genthial, Octobre 2002, d'après la primitive
 * ouvreConnexion de André Lagrèze (Octobre 2000)
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define RESEAU_C
#include "Reseau.h"

/* socketServer : initialise un serveur grâce à un socket sur le
 * port donné. Initialisation sur le domaine AF_INET selon le protocole.
 * Entrées : numéro du port, protocole
 * Sorties : le numéro du socket ou -1 en cas d'erreur (des messages sont 
 *           affichés par perror).
 *   En mode TCP, le serveur est prêt à accepter des connexions avec :
 *              msgSock = accept(num, NULL, NULL);
 *              ...
 *              close(msgSock);
 *   NB : On peut se payer le luxe d'ignorer les détails renvoyés par accept 
 *   (les deux pointeurs NULL) car seul le numéro du socket nous intéresse,
 *   pour faire read et write.
 *   En mode UDP, le socket est prêt pour lecture/écriture 
 *   avec sendto/recvfrom . */
int socketServer(unsigned short int port, Protocole p)
{
   int numSocket;

   /* Pour une connexion de type IP/IP (AF_INET), on utilise sockaddr_in
    * (sockaddr est la forme générique, utilisée uniquement pour rendre les
    * fonctions génériques) */
   struct sockaddr_in server;

   /* Création du socket, mode IP/IP, STREAM ou DGRAM selon p, le protocole
    * (arg 3) est IP (voir /etc/protocols) */
   if (p == TCP)
      numSocket = socket(AF_INET, SOCK_STREAM, 0);
   else
      numSocket = socket(AF_INET, SOCK_DGRAM, 0);
   if (numSocket < 0)
   {
      perror("Création du socket");
      return -1;
   }

   /* Cas d'un serveur on fait un bind pour lier le socket au port, sur une
    * adresse donnée (de la machine courante) */
   server.sin_family = AF_INET;
   /* On lie pour toutes les adresses de la machine courante */
   server.sin_addr.s_addr = htonl(INADDR_ANY);
   /* Notez l'utilisation des primitives htonX pour garantir que l'adresse ou
    * le numéro de port seront bien stockés au format réseau (big endian) */
   server.sin_port = htons(port);
   if (bind(numSocket, (struct sockaddr *) &server, sizeof(server)) < 0)
   {
      perror("bind");;
      return -1;
   }

   if (p == TCP)
   {
      /* On place le socket en mode écoute, file d'attente de longueur 5 */
      if (listen(numSocket, 5) < 0)
      {
         perror("listen");
         return -1;
      }
   }

   /* Il ne reste plus qu'à attendre les connexions avec accept(...), qui
    * créera un nouveau socket pour la communication à chaque nouvelle
    * connexion d'un client */

   return (numSocket);
}

/* socketClient : ouvre une communication vers un serveur grâce à un socket.
 * Initialisation sur le domaine AF_INET selon le protocole.
 * Entrées : nom du serveur, numéro du port et protocole.
 * Sorties : le numéro du socket ou -1 en cas d'erreur (des messages sont 
 *           affichés par perror).
 *    En mode TCP : le socket est prêt à accepter des lectures/écriture avec :
 *              read(numSocket, &tampon, sizeof(tampon);
 *              write(numSocket, &tampon, sizeof(tampon);
 *              ...
 *              close(numSocket);
 *   En mode UDP, le socket est prêt pour lecture/écriture 
 *   avec sendto/recvfrom . */
int socketClient(const char nomServeur[], unsigned short int port, Protocole p)
{
   int numSocket;
   struct sockaddr_in client;
   struct hostent *hp;      /* Pour la conversion nom->adresseIP */

   /* Création du socket, mode IP/IP, STREAM ou DGRAM selon p, le protocole
    * (arg 3) est IP (voir /etc/protocols) */
   if (p == TCP)
      numSocket = socket(AF_INET, SOCK_STREAM, 0);
   else
      numSocket = socket(AF_INET, SOCK_DGRAM, 0);
   if (numSocket < 0)
   {
      perror("Création du socket");
      return -1;
   }

   /* Dans le cas du client, on utilise le port auquel le socket est
    * automatiquement lié à la création. */

   /* Conversion nom->adresseIP */
   hp = gethostbyname(nomServeur);
   if (hp == (struct hostent *) NULL)
   {
      fprintf(stderr, "%s: serveur inconnu\n", nomServeur);
      return -1;
   }
   memcpy((char *) &client.sin_addr, (char *) hp->h_addr_list[0], hp->h_length);

   if (p == TCP)
   {
      /* Connexion au serveur distant */
      client.sin_family = AF_INET;
      client.sin_port = htons(port);
      if (connect(numSocket, (struct sockaddr *) &client, sizeof client) == -1)
      {
         perror("Connexion au socket en mode STREAM");
         return -1;
      }
   }

   /* Si la connexion a réussi, on n'a plus qu'à faire des read et write sur
    * numSocket */
   return (numSocket);
}

/* afficheInfosSocket : affiche les informations d'un socket.
 * Entrées : le numéro du socket
 * Sorties : sur stdout. */
void afficheInfosSocket(int numSocket)
{
   socklen_t adrLen;
   struct sockaddr_in sockLocal, sockDist;
   adrLen = sizeof(struct sockaddr_in);
   printf("Informations sur socket n°%d \n", numSocket);
   if (getsockname(numSocket, (struct sockaddr *) &sockLocal, &adrLen) != 0)
      printf("Problème sur partie locale du socket\n");
   else
      printf("@IP locale:%s  port local:%d\n", inet_ntoa(sockLocal.sin_addr),
        ntohs(sockLocal.sin_port));

   /* On n'affiche qu'en cas de succès pour la partie distante */
   if (getpeername(numSocket, (struct sockaddr *) &sockDist, &adrLen) == 0)
      printf("@IP distante:%s  port distant: %d \n",
        inet_ntoa(sockDist.sin_addr), ntohs(sockDist.sin_port));
}
