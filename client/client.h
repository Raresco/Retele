#ifndef __CLIENT_H
#define __CLIENT_H
#include <poll.h>

struct Client
{
    int sd; // descriptorul de socket
    struct pollfd fds[1];
    char player;
    char *user1;
    char *user2;
    char reversiTable[8][8];
    char tura;
};

void inchide_client(struct Client *pclient);
int connectare_la_server(struct Client *pclient, const char *addr, int port);
void citeste_jucator(struct Client *pclient,char user1);
int valideaza_mutare(struct Client *pclient, char a, char b, char *valid);
int este_randul_nostru(struct Client *pclient);
int citeste_tura(struct Client *pclient);
#endif
