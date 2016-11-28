/* Module pour l'ouverture de connexions UDP ou TCP client ou serveur
 * Auteur : Damien Genthial, Octobre 2002, d'après la primitive
 * ouvreConnexion de André Lagrèze (Octobre 2000)
 */

#ifndef RESEAU_H
#define RESEAU_H

/* Définition des protocoles possibles */
typedef enum { UDP, TCP } Protocole;

#ifndef RESEAU_C
#define PUBLIC extern
#else
#define PUBLIC
#endif

/* socketServer : initialise un serveur grâce à un socket sur le
 * port donné. Initialisation sur le domaine AF_INET selon le protocole.
 * Entrées : numéro du port, protocole
 * Sorties : le numéro du socket ou -1 en cas d'erreur (des messages sont 
 *           affichés par perror).
 *   En mode TCP, le serveur est prêt à accepter des connexions avec :
 *              msgSock = accept(num, NULL, NULL);
 *              ...
 *              close(msgSock);
 *   En mode UDP, le socket est prêt pour lecture/écriture 
 *   avec sendto/recvfrom . */
PUBLIC int socketServer(unsigned short int port, Protocole proto);

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
PUBLIC int socketClient(const char nomServeur[], unsigned short int port, Protocole p);

/* afficheInfosSocket : affiche les informations d'un socket.
 * Entrées : le numéro du socket
 * Sorties : sur stdout. */
PUBLIC void afficheInfosSocket(int numSocket);

#undef PUBLIC
#endif /* #ifndef RESEAU_H */
