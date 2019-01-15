#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

#include "client.h"

void inchide_client(struct Client *pclient)
{
    close(pclient->sd);
}

int connectare_la_server(struct Client *pclient, const char *addr, int port)
{
    struct sockaddr_in server; // structura folosita pentru conectare

    if ((pclient->sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(addr);
    server.sin_port = htons(port);

    if (connect(pclient->sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }
    pclient->fds[0].fd = pclient->sd;
    pclient->fds[0].events = POLLIN;
    pclient->user1 = "";
    pclient->user2 = "";
    return 0;
}

void citeste_jucator(struct Client *pclient)
{
    read(pclient->sd, &pclient->player, 1);
    // error handling ???
    if (pclient->player == 0){
        pclient->user1 = "eu";
        pclient->user2 = "dusmanul";
    }else{
        pclient->user2 = "eu";
        pclient->user1 = "dusmanul";
    }
}

int este_randul_nostru(struct Client *pclient)
{
    return pclient->tura % 2 == pclient->player;
}

int citeste_tura(struct Client *pclient)
{
    int err = poll(pclient->fds, 1, 0);
    if (err == 0)
        return 1;
    if (err == -1)
        return err;
    read(pclient->sd, &pclient->tura, 1);
    printf("Este tura cu numarul %d \n", pclient->tura);
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (read(pclient->sd, &pclient->reversiTable[i][j], 1) <= 0)
            {
                perror("Nu s-a putut prealua tabla de joc.");
                return errno;
            }
            printf("%d ", pclient->reversiTable[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int valideaza_mutare(struct Client *pclient, char a, char b, char *valid)
{
    if (write(pclient->sd, &a, 1) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
        return errno;
    }

    if (write(pclient->sd, &b, 1) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
        return errno;
    }

    if (read(pclient->sd, valid, 1) <= 0)
    {
        perror("[client]Eroare la read() de la server.\n");
        return errno;
    }
    printf("%d \n", *valid);
    return 0;
}
