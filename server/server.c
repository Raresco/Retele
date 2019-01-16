#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define PORT 2114


struct gameState{
    char playerPoints[2];
    char possibleMove;
    char winner;
};


typedef struct sockaddr_in sockaddr_in;
typedef struct gameState gameState;

extern int errno;
char reversiTable[8][8];
int client[2];
sockaddr_in server;
sockaddr_in from;
int sd;


char rightNeighbour(char reversiTable[8][8], char playerNo, char xCoordinate, char yCoordinate){


    if(yCoordinate!=7)
    {
        
        if((reversiTable[xCoordinate][yCoordinate+1] != (playerNo+1)) && (reversiTable[xCoordinate][yCoordinate+1]!=0))
            for(int k = yCoordinate + 1; k <= 7; ++k)
            {
                if((reversiTable[xCoordinate][k] == (playerNo +1)))
                    return k;
                if(!reversiTable[xCoordinate][k])
                    return -1;
                    
            }
    }
    return -1;
}

char leftNeighbour(char reversiTable[8][8], char playerNo, char xCoordinate, char yCoordinate){
     if(yCoordinate!=0)
    {
        if((reversiTable[xCoordinate][yCoordinate-1] != (playerNo+1)) && (reversiTable[xCoordinate][yCoordinate-1]!=0))
            for(int k = yCoordinate - 1; k >= 0 ; --k)
            {
                if((reversiTable[xCoordinate][k] == (playerNo +1)))
                    return k;
                if(!reversiTable[xCoordinate][k])
                        return -1;
            }
    }
    return -1;
    
}

char lowerNeighbour(char reversiTable[8][8], char playerNo, char xCoordinate, char yCoordinate){
    if(xCoordinate != 7){
        if(reversiTable[xCoordinate+1][yCoordinate] != (playerNo+1))
            for(int k = xCoordinate + 1; k <= 7; ++k)
            {
                if(reversiTable[k][yCoordinate] == (playerNo + 1))
                   return k;
                if(!reversiTable[k][yCoordinate])
                    return -1;
            }

    }
    return -1;
}

char upperNeighbour(char reversiTable[8][8], char playerNo, char xCoordinate, char yCoordinate){
    if(xCoordinate != 0){
        if((reversiTable[xCoordinate - 1][yCoordinate]) != (playerNo + 1))
            for(int k = xCoordinate - 1; k >= 0; --k){
                if(reversiTable[k][yCoordinate] == (playerNo + 1))
                    return k;
                if(!reversiTable[k][yCoordinate])
                    return -1;
            }
    }
    return -1;
}

char* leftUpper(char reversiTable[8][8], char playerNo, char xCoordinate, char yCoordinate){
    char* coordinates = (char*)malloc(2);
	*coordinates = -1;
	*(coordinates + 1) = -1;
    if((xCoordinate !=0) && (yCoordinate !=0))
    {
        char auxReverse = 0;
        char tempX, tempY;
        if((reversiTable[xCoordinate-1][yCoordinate - 1] != (playerNo+1)) && (reversiTable[xCoordinate-1][yCoordinate - 1]!=0))
        {
            if( xCoordinate < yCoordinate  )
            {
                tempX = yCoordinate - xCoordinate;
                tempY = 0;
            }
            else
            {
                tempX = 0;
                tempY = yCoordinate - xCoordinate;
            }
            printf("%d %d \n", tempX, tempY);
            for(char i = xCoordinate - 1, j = yCoordinate -1; i >= tempX && j >= tempY; i--, j--)
            {
                printf("%d %d \n",i,j);
                if(reversiTable[i][j] == (playerNo + 1)){
                    *coordinates= i;
                    *(coordinates + 1) = j;
                    printf("leftUpperBound: %d %d \n",i,j);
                    return coordinates;
                }
                    
                if(!reversiTable[i][j])
                    return coordinates;
            }
        }
    }
    return coordinates;

}

char* rightUpper(char reversiTable[8][8], char playerNo, char xCoordinate, char yCoordinate){
    char* coordinates = (char*)malloc(2);
    *coordinates = -1;
    *(coordinates + 1) = -1;
    if((xCoordinate != 0) && (yCoordinate != 7))
    {
        char tempX, tempY;
        if((reversiTable[xCoordinate-1][yCoordinate+1]!= (playerNo + 1)) && (reversiTable[xCoordinate-1][yCoordinate+1]!=0))
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
            for( char i = xCoordinate - 1, j = yCoordinate + 1; i >= tempX && j <= tempY; i--, j++)
            {
                if(reversiTable[i][j] == (playerNo + 1)){
                    *(coordinates) = i;
                    *(coordinates + 1) = j;
                    printf("RightUpperBound: %d %d \n", i,j);
                    return coordinates;
                }
                if(!reversiTable[i][j])
                    return coordinates;
                    
                    
            }

        }
    }
    return coordinates;    
}

char* leftLower(char reversiTable[8][8], char playerNo, char xCoordinate, char yCoordinate){
    char* coordinates = (char*)malloc(2);
    *coordinates = -1;
    *(coordinates + 1) = -1;
    if((xCoordinate !=7)&&(yCoordinate != 0)){
        char tempX, tempY;
        if((reversiTable[xCoordinate + 1][yCoordinate - 1] != (playerNo + 1)) && (reversiTable[xCoordinate + 1][yCoordinate - 1] != 0))
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
            for(char i = xCoordinate + 1, j = yCoordinate - 1; i <= tempX && j >= tempY; i++, j--)
            {
                if(reversiTable[i][j] == (playerNo + 1))
                {
                    *coordinates = i;
                    *(coordinates + 1) = j;
                    printf("leftLowerBound: %d %d \n",i,j);
                    return coordinates;
                }
                if(!reversiTable[i][j])
                    return coordinates;
            }
        }

    }
    return coordinates;
}

char* rightLower(char reversiTable[8][8], char playerNo, char xCoordinate, char yCoordinate){
    char* coordinates = (char*)malloc(2);
    *coordinates = -1;
    *(coordinates + 1) = -1;
    if((xCoordinate !=7)&&(yCoordinate !=7))
    {
        
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

            for(char i = xCoordinate + 1, j = yCoordinate + 1; i <= tempX, j <= tempY; i++, j++)
            {
                if(reversiTable[i][j] == (playerNo + 1))
                {
                    *(coordinates) = i;
                    *(coordinates + 1) = j;
                    printf("rightLowerBound: %d %d \n",i,j);
                    return coordinates;
                }
                if(!reversiTable[i][j])
                    return coordinates;
            }
        }

    }
    return coordinates;

}
char doesMoveExist(char reversiTable[8][8], char tura, gameState* currentGS){

    if( tura <= 3){
        for(int i = 3; i <= 4; ++i){
            for(int j = 3; j<= 4; ++j)
                if(reversiTable[i][j] == ((tura % 2) + 1))
                    (currentGS->playerPoints[tura % 2])++;
        }
        return 1;
    }
    char exists = -1;

    for(int i = 0; i <= 7; ++i){
        for( int j = 0; j<= 7; ++j){
            if(reversiTable[i][j]==((tura % 2) + 1))
                (currentGS->playerPoints[tura % 2])++;
			if(!exists){
				if(lowerNeighbour(reversiTable, (tura % 2), i, j)){
					exists = 1;
				}
				if(upperNeighbour(reversiTable, (tura % 2), i, j)){
					exists = 1;
				}
				if(leftNeighbour(reversiTable, (tura % 2), i, j)){
					exists = 1;
				}
				if(rightNeighbour(reversiTable, (tura % 2), i, j)){
					exists = 1;
				}

				char* Coordinates;

				Coordinates = rightUpper(reversiTable, (tura % 2), i, j);
				if(*Coordinates)
					exists = 1;
				free(Coordinates);
				Coordinates = rightLower(reversiTable, (tura % 2), i, j);
				if(*Coordinates)
					exists = 1;
				free(Coordinates);
				Coordinates = leftLower(reversiTable, (tura % 2), i, j);
				if(*Coordinates)
					exists = 1;
				free(Coordinates);
				Coordinates = leftUpper(reversiTable, (tura % 2), i, j);
				if(*Coordinates)
					exists = 1;
				free(Coordinates);

            
			}   
		}
	}
	if(!exists)
		currentGS->possibleMove = 0;
	
	return 1;
}



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
	
    char tempY = leftNeighbour(reversiTable, playerNo, xCoordinate, yCoordinate);
	if(tempY != -1){
		for(int j = tempY; j <= yCoordinate ; ++j)
			reversiTable[xCoordinate][j] = (playerNo + 1);
	reverse = 1;
	}
	tempY = rightNeighbour(reversiTable, playerNo, xCoordinate, yCoordinate);
	if(tempY != -1){
		for(int j = tempY; j >=yCoordinate; --j)
			reversiTable[xCoordinate][j] = (playerNo + 1);
	reverse = 1;
	}

    char tempX = upperNeighbour(reversiTable, playerNo, xCoordinate, yCoordinate);
	if(tempX != -1){
		for(int i = tempX; i <= xCoordinate; ++i)
			reversiTable[i][yCoordinate] = (playerNo + 1);
	reverse = 1;
	}
	
	tempX = lowerNeighbour(reversiTable, playerNo, xCoordinate, yCoordinate);
	if(tempX != -1){
		for(int i = tempX; i >= xCoordinate; --i)
			reversiTable[i][yCoordinate] = (playerNo + 1);
	reverse = 1;
	}  
	
	char* temporaryCoordinates = leftLower(reversiTable, playerNo, xCoordinate, yCoordinate);
	tempX = *temporaryCoordinates;
	tempY = *(temporaryCoordinates + 1);
	if((*temporaryCoordinates) != -1){
		for(char i = tempX, j = tempY; i >= xCoordinate, j <= yCoordinate; i--, j++)
			reversiTable[i][j] = (playerNo + 1);
	reverse = 1;
	}
	free(temporaryCoordinates);
    
	temporaryCoordinates = rightLower(reversiTable, playerNo, xCoordinate, yCoordinate);
	if((*(temporaryCoordinates)) != -1){
		for(char i = *temporaryCoordinates, j = (*(temporaryCoordinates + 1)); i >= xCoordinate, j >= yCoordinate; i--, j--)
			reversiTable[i][j] = (playerNo + 1);
	reverse = 1;
	}
	free(temporaryCoordinates);

	temporaryCoordinates = rightUpper(reversiTable, playerNo, xCoordinate, yCoordinate);
	if((*(temporaryCoordinates)) != -1){
		for(char i = *temporaryCoordinates, j = (*(temporaryCoordinates + 1)); i <= xCoordinate, j >= yCoordinate; i++, j--)
			reversiTable[i][j] = (playerNo + 1);
	reverse = 1;
	}
	free(temporaryCoordinates);

	temporaryCoordinates = leftUpper(reversiTable, playerNo, xCoordinate, yCoordinate);
    if((*(temporaryCoordinates)) != -1){
		for(char i = *temporaryCoordinates, j = (*(temporaryCoordinates + 1)); i <= xCoordinate, j <= yCoordinate; i++, j++)
			reversiTable[i][j] = (playerNo + 1);
	reverse = 1;
	}
	free(temporaryCoordinates);	 
	

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

void getClientMove(int client[2], char reversiTable[8][8], char* contor, gameState* currentGS){
    char xCoordinate = 0;
    char yCoordinate = 0;
    char valid = 0;
    if(doesMoveExist(reversiTable, *contor, currentGS)){
        do
            {
                if (read (client[(*contor)%2], &xCoordinate, 1) <= 0)
                    {
                //     perror ("[server]Eroare la read() de la client.\n");
                    continue;
                    }
                if(read(client[(*contor)%2], &yCoordinate, 1) <= 0)
                {
                //     perror("[server]Eroare la read() de la client. \n");
                    continue;
                }
                    valid = validMove(xCoordinate,yCoordinate,(*contor)%2, reversiTable, *contor);
                    printf("%d \n", valid);
                    write(client[(*contor)%2], &valid, 1);
            }while(!valid);
            (*contor)++;
    }
    else{
        if(currentGS->playerPoints[0] > currentGS->playerPoints[1])
            currentGS->winner = 0;
        else if(currentGS->playerPoints[1] > currentGS->playerPoints[0])
            currentGS->winner = 1;
        else currentGS->winner = 2;
    }

}

void initializeGame(gameState* GS){
    GS->winner = -1;
    GS->playerPoints[0] = 0;
    GS->playerPoints[1] = 0;
    GS->possibleMove = 1;
}

void sendScores(gameState* GS, int client[2]){

        for(int i = 0; i <= 1; ++i)
            for(int j = 0; j <=1; ++j)
                write(client[i], &(GS->playerPoints[j]), 1);

}

void sendPlayerNumber(int client[2]){
    for(int i = 0; i < 2; ++i)
        write(client[i], &i, 1);
}
void updateTurn(int client[2], char contor){
    for(int i = 0; i < 2; ++i)
        write(client[i], &contor, 1);
}
void getUsernames(int client[2], char* usernames[2]){
     
    char lungime = 0;
    for(int i = 0; i <= 1; ++i){
        read(client[i], &lungime, 1);
        usernames[i] = (char*)malloc(lungime + 1);
        read(client[i], usernames[i], (lungime + 1));
        lungime = 0;
    }
}
void sendOpponentUsername(int client[2], char* usernames[2]){
    char lungime;
    for(int i = 0; i <= 1; ++i){
        lungime = strlen(usernames[(i + 1) % 2]) + 1;
        write(client[i], &lungime, 1);
        write(client[i], usernames[(i + 1) % 2], lungime);

    }
}
int main ()
{
    deployServer(&server, &from, &sd);   
    int clientiConectati = 0;
    char* usernames[2];
    while (1)
    {
        int pid = -1;
        int length = sizeof(from);
        printf ("[server]Asteptam la portul %d...\n",PORT);
        fflush (stdout);
        if((client[clientiConectati++] = accept (sd, (struct sockaddr *) &from, &length)) <= 0)
        {
            perror("[server]Eroare la conectare.\n");
            
            clientiConectati = (clientiConectati + 1) % 2;
            continue;
        }
        if(clientiConectati == 2)
        {   
            gameState currentGS;
            initializeGame(&currentGS);

            if( (pid = fork()) == -1)
            {
                perror("[server]Nu s-a putut creea camera de joc.\n");
                cleanDescriptors(client, &clientiConectati);
                continue;
            }
            else if(pid == 0)
            {
                char contor = 0;
                sendPlayerNumber(client);
                getUsernames(client, usernames);
                sendOpponentUsername(client, usernames);
                while(1)
                {
                    updateTurn(client, contor);
                    sendMatrix(client, reversiTable);
              //      sendScores(&currentGS, client);
                    getClientMove(client, reversiTable, &contor, &currentGS);
                 //   sendMatrix(client, reversiTable);

                }
            }
            else if(pid)
            {
                cleanDescriptors(client, &clientiConectati);

            }
        }


    }

}
