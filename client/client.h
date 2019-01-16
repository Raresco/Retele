#ifndef __CLIENT_H
#define __CLIENT_H
#include <poll.h>
#define MAX_USERNAME 20
struct Client
{
    int sd; // descriptorul de socket
    struct pollfd fds[1];
    char player;
    char user1[MAX_USERNAME+1];
    char user2[MAX_USERNAME+1];
    char reversiTable[8][8];
    char tura;
};

void inchide_client(struct Client *pclient);
int connectare_la_server(struct Client *pclient, const char *addr, int port);
void citeste_jucator(struct Client *pclient);
int valideaza_mutare(struct Client *pclient, char a, char b, char *valid);
int este_randul_nostru(struct Client *pclient);
int citeste_tura(struct Client *pclient);
void ia_numele_adversarului(struct Client *pclient);
void da_numele_serverului(struct Client *pclient);
#endif
