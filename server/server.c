#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define PORT 2097


typedef struct sockaddr_in sockaddr_in;


extern int errno;
char reversiTable[8][8];
int client[2];
sockaddr_in server;
sockaddr_in from;
int sd;


char earlyMove(char xCoordinate, char yCoordinate, char reversiTable[8][8], char tura)
{
    if(reversiTable[xCoordinate][yCoordinate]!=0)
        return 0;
    if( xCoordinate!=3 && xCoordinate!=4)
        return 0;
    if( yCoordinate!=3 && yCoordinate!=4)
        return 0;
    if( reversiTable[xCoordinate-1][yCoordinate-1] == (tura%2 +1) || reversiTable[xCoordinate+1][yCoordinate+1] == (tura%2 + 1))
        return 0;
    if( reversiTable[xCoordinate+1][yCoordinate-1] == (tura%2 +1) || reversiTable[xCoordinate-1][yCoordinate+1] == (tura%2 + 1))
        return 0;
    return 1;
}

char validMove(char xCoordinate, char yCoordinate,char playerNo, char reversiTable[8][8], char tura)
{

    char reverse = 0;
    if(reversiTable[xCoordinate][yCoordinate]!=0)
        return reverse;
    if( tura <= 3)
        if((reverse = earlyMove(xCoordinate, yCoordinate, reversiTable, tura))!=0)
        {
            reversiTable[xCoordinate][yCoordinate] = playerNo + 1;
            return reverse;
        }
    if(yCoordinate!=7)
    {
        char auxReverse = 0;
        if((reversiTable[xCoordinate][yCoordinate+1] != (playerNo+1)) && (reversiTable[xCoordinate][yCoordinate+1]!=0))
            for(int k = 7; k >= yCoordinate; --k)
            {
                if(reversiTable[xCoordinate][k] == (playerNo +1))
                    auxReverse = reverse = 1;
                if(auxReverse == 1)
                    reversiTable[xCoordinate][k] = playerNo +1;
            }
    }
    if(yCoordinate!=0)
    {
        char auxReverse = 0;
        if((reversiTable[xCoordinate][yCoordinate-1] != (playerNo+1)) && (reversiTable[xCoordinate][yCoordinate-1]!=0))
            for(int k = 0; k <= yCoordinate; ++k)
            {
                if(reversiTable[xCoordinate][k] == (playerNo +1))
                    auxReverse =	reverse = 1;
                if(auxReverse == 1)
                    reversiTable[xCoordinate][k] = playerNo +1;
            }
    }
    if(xCoordinate!=0)
    {
        char auxReverse = 0;
        if((reversiTable[xCoordinate-1][yCoordinate] != (playerNo+1)) && (reversiTable[xCoordinate-1][yCoordinate]!=0))
            for(int k = 0; k <= xCoordinate; ++k)
            {
                if(reversiTable[k][yCoordinate] == (playerNo + 1))
                    auxReverse = reverse = 1;
                if(auxReverse == 1)
                    reversiTable[k][yCoordinate] = playerNo + 1;
            }
    }
    if(xCoordinate!=7)
    {
        char auxReverse = 0;
        if((reversiTable[xCoordinate+1][yCoordinate] != (playerNo+1)) && (reversiTable[xCoordinate+1][yCoordinate]!=0))
            for(int k = 7; k >= xCoordinate; --k)
            {
                if(reversiTable[k][yCoordinate] == (playerNo + 1))
                    auxReverse = reverse = 1;
                if(auxReverse == 1)
                    reversiTable[k][yCoordinate] = playerNo + 1;
            }
    }
    if((xCoordinate !=0) && (yCoordinate !=7))
    {
        char auxReverse = 0;
        char tempX, tempY;
        if((reversiTable[xCoordinate-1][yCoordinate+1] != (playerNo+1)) && (reversiTable[xCoordinate-1][yCoordinate+1]!=0))
        {
            if(( xCoordinate + yCoordinate  ) <=7)
            {
                tempX = 0;
                tempY = xCoordinate + yCoordinate;
            }
            else
            {
                tempX = yCoordinate - 1;
                tempY = (xCoordinate + yCoordinate) - 7;
            }
            printf("%d %d \n", tempX, tempY);
            for(char i = tempX, j = tempY; i <= xCoordinate && j >= yCoordinate; i++, j--)
            {
                printf("%d %d \n",i,j);
                if(reversiTable[i][j] == (playerNo + 1))
                    auxReverse = reverse = 1;
                if(auxReverse == 1)
                    reversiTable[i][j] = playerNo + 1;
            }
        }
    }
    if((xCoordinate != 7) && (yCoordinate != 0))
    {
        char auxReverse = 0;
        char tempX, tempY;
        if((reversiTable[xCoordinate+1][yCoordinate-1]!= (playerNo + 1)) && (reversiTable[xCoordinate+1][yCoordinate-1]!=0))
        {
            if((xCoordinate + yCoordinate) <= 7)
            {
                tempX = xCoordinate + yCoordinate;
                tempY = 0;
            }
            else
            {
                tempX = (xCoordinate + yCoordinate) - 7;
                tempY = xCoordinate + 1;
            }
            printf("%d %d \n", tempX, tempY);
            for( char i = tempX, j = tempY; i >= xCoordinate && j <= yCoordinate; i--, j++)
            {
                if(reversiTable[i][j] == (playerNo + 1))
                    auxReverse = reverse = 1;
                if(auxReverse == 1)
                    reversiTable[i][j] = playerNo + 1;
            }

        }
    }
    if((xCoordinate != 0) && (yCoordinate !=0))
    {
        char auxReverse = 0;
        char tempX, tempY;
        if((reversiTable[xCoordinate - 1][yCoordinate - 1] != (playerNo + 1)) && (reversiTable[xCoordinate - 1][yCoordinate - 1] != 0))
        {
            if(xCoordinate < yCoordinate)
            {
                tempX = xCoordinate - yCoordinate;
                tempY = 0;
            }
            else
            {
                tempX = 0;
                tempY = yCoordinate - xCoordinate;
            }
            for(char i = tempX, j = tempY; i <= xCoordinate, j <= yCoordinate; i++, j++)
            {
                if(reversiTable[i][j] == (playerNo + 1))
                {
                    auxReverse = reverse = 1;
                    printf("merge stanga sus \n");
                }
                if(auxReverse == 1)
                    reversiTable[i][j] = playerNo + 1;
            }
        }
    }
    if((xCoordinate !=7)&&(yCoordinate !=7))
    {
        char auxReverse = 0;
        char tempX, tempY;
        if((reversiTable[xCoordinate +1 ][yCoordinate + 1] != (playerNo + 1)) && (reversiTable[xCoordinate + 1][yCoordinate + 1] != 0))
        {
            if(xCoordinate <= yCoordinate)
            {
                tempX = 7 -(yCoordinate - xCoordinate);
                tempY = 7;
            }
            else
            {
                tempX = 7;
                tempY = 7 - (xCoordinate - yCoordinate);
            }
            printf("%d %d \n", tempX, tempY);

            for(char i = tempX, j = tempY; i >= xCoordinate, j >= yCoordinate; i--, j--)
            {
                if(reversiTable[i][j] == (playerNo + 1))
                {
                    auxReverse = reverse = 1;
                    printf("merge stanga jos \n");
                }
                if(auxReverse == 1)
                    reversiTable[i][j] = playerNo + 1;
            }
        }

    }

    return reverse;
}

int deployServer(sockaddr_in* server, sockaddr_in* from, int* sd){
    if ((*sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server]Eroare la socket().\n");
        exit(3);
        return errno;
    }

    bzero (server, sizeof (*server));
    bzero (from, sizeof (*from));

    server->sin_family = AF_INET;
    server->sin_addr.s_addr = htonl (INADDR_ANY);
    server->sin_port = htons (PORT);

    if (bind (*sd, (struct sockaddr *) server, sizeof (struct sockaddr)) == -1)
    {
        perror ("[server]Eroare la bind().\n");
        exit(1);
        return errno;
    }

    if (listen (*sd, 50) == -1)
    {
        perror ("[server]Eroare la listen().\n");
        exit(2);
        return errno;
    }
}

void cleanDescriptors(int clienti[2], int* clientiConectati){
     *clientiConectati = 0;
     for(int i = 0; i <= 1; ++i)
            client[i] = 0;
}

int sendMatrix(int clienti[2], char reversiTable[8][8]){
     for(int nr = 0; nr <= 1; ++nr)
                    {
                        for(int i = 0; i < 8; ++i)
                            for(int j = 0; j < 8; ++j)
                            {
                                if( write(client[nr], &reversiTable[i][j],1)<=0)
                                {
                                    perror("[server]Eroare la transmiterea matricei.");
                                    exit(4);
                                    return errno;
                                }
                            }
                    }
    return 1; 
}

void getClientMove(int client[2], char reversiTable[8][8], char contor){
    char xCoordinate = 0;
    char yCoordinate = 0;
    char valid = 0;
    do
        {
             if (read (client[contor%2], &xCoordinate, 1) <= 0)
                {
                   perror ("[server]Eroare la read() de la client.\n");
                   continue;
                }
             if(read(client[contor%2], &yCoordinate, 1) <= 0)
               {
                  perror("[server]Eroare la read() de la client. \n");
                  continue;
               }
                valid = validMove(xCoordinate,yCoordinate,contor%2, reversiTable, contor);
                printf("%d \n", valid);
                write(client[contor%2], &valid, 1);
        }while(!valid);
}

void sendPlayerNumber(int client[2]){
    for(int i = 0; i < 2; ++i)
                    write(client[i], &i, 1);
}
void updateTurn(int client[2], char contor){
    for(int i = 0; i < 2; ++i)
                        write(client[i], &contor, 1);
}
int main ()
{
    deployServer(&server, &from, &sd);   
    int clientiConectati = 0;
    while (1)
    {
        int pid = -1;
        int length = sizeof(from);
        printf ("[server]Asteptam la portul %d...\n",PORT);
        fflush (stdout);
        if((client[clientiConectati++] = accept (sd, (struct sockaddr *) &from, &length)) <= 0)
        {
            perror("[server]Eroare la conectare.\n");
            return errno;
        }
        if(clientiConectati == 2)
        {
            if( (pid = fork()) == -1)
            {
                perror("[server]Nu s-a putut creea camera de joc.\n");
                return -1;
            }
            else if(pid == 0)
            {
                char contor = 0;
                sendPlayerNumber(client);
                while(1)
                {
                    updateTurn(client, contor);
                    sendMatrix(client, reversiTable);
                    getClientMove(client, reversiTable, contor);
                    contor++;
                    sendMatrix(client, reversiTable);

                }
            }
            else if(pid)
            {
                cleanDescriptors(client, &clientiConectati);

            }
        }


    }

}
