#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT 2077

extern int errno;
char reversiTable[8][8];
int client[2];

char earlyMove(char xCoordinate, char yCoordinate, char reversiTable[8][8], char tura){
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

char validMove(char xCoordinate, char yCoordinate,char playerNo, char reversiTable[8][8], char tura){
  
	char reverse = 0;
	if(reversiTable[xCoordinate][yCoordinate]!=0)
		return reverse;
	if( tura <= 3)
		  if((reverse = earlyMove(xCoordinate, yCoordinate, reversiTable, tura))!=0){
				reversiTable[xCoordinate][yCoordinate] = playerNo + 1;
				return reverse;
			}
	if(yCoordinate!=7){
		if((reversiTable[xCoordinate][yCoordinate+1] != (playerNo+1)) && (reversiTable[xCoordinate][yCoordinate+1]!=0))
			for(int k = 7; k >= yCoordinate; --k){
				if(reversiTable[xCoordinate][k] == (playerNo +1))
					reverse = 1;
				if(reverse == 1)
					reversiTable[xCoordinate][k] = playerNo +1;
			}
		if(yCoordinate!=0){
		if((reversiTable[xCoordinate][yCoordinate-1] != (playerNo+1)) && (reversiTable[xCoordinate][yCoordinate-1]!=0))
			for(int k = 0; k <= yCoordinate; ++k){
				if(reversiTable[xCoordinate][k] == (playerNo +1))
					reverse = 1;
				if(reverse == 1)
					reversiTable[xCoordinate][k] = playerNo +1;
			}
		if(xCoordinate!=0){
					if((reversiTable[xCoordinate-1][yCoordinate] != (playerNo+1)) && (reversiTable[xCoordinate-1][yCoordinate]!=0))
						for(int k = 0; k <= xCoordinate; ++k){
							if(reversiTable[k][yCoordinate] == (playerNo + 1))
								reverse = 1;
							if(reverse == 1)
								reversiTable[k][yCoordinate] = playerNo + 1;
						}
		}
    if(xCoordinate!=7){
			if((reversiTable[xCoordinate+1][yCoordinate] != (playerNo+1)) && (reversiTable[xCoordinate+1][yCoordinate]!=0))
						for(int k = 7; k >= xCoordinate; --k){
							if(reversiTable[k][yCoordinate] == (playerNo + 1))
								reverse = 1;
							if(reverse == 1)
								reversiTable[k][yCoordinate] = playerNo + 1;
						}
		}

	}
	}
	return reverse;
		}

	 

int main ()
{
  struct sockaddr_in server;	
  struct sockaddr_in from;	
  int sd;			 
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }

  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
    server.sin_family = AF_INET;	
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);
  
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  int clientiConectati = 0;
  while (1)
    {
      int length = sizeof(from);
      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);
      if((client[clientiConectati++] = accept (sd, (struct sockaddr *) &from, &length)) <= 0){
		      perror("[server]Eroare la conectare.\n");
		      return errno;
      }
      if(clientiConectati == 2){
     	      char contor = 0;
	      for(int i = 0; i < 2; ++i)
		      write(client[i], &i, 1);
	      while(1){
        for(int i = 0; i < 2; ++i)
          write(client[i], &contor, 1);
	      for(int nr = 0; nr <= 1; ++nr){
	     	 for(int i = 0; i < 8; ++i)
		      for(int j = 0; j < 8; ++j){
			     if( write(client[nr], &reversiTable[i][j],1)<=0){
				     perror("[server]Eroare la transmiterea matricei.");
				     return errno;
			     }
		      }
		}
	      char xCoordinate = 0;
	      char yCoordinate = 0; 
		char valid;
    do{   
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
		}while(valid == 0); 
		validMove(xCoordinate, yCoordinate, contor%2, reversiTable, contor);
	     contor++;
       	     for(int nr = 0; nr <= 1; ++nr){
	     	 for(int i = 0; i < 8; ++i)
		      for(int j = 0; j < 8; ++j){
			     if( write(client[nr], &reversiTable[i][j],1)<=0){
				     perror("[server]Eroare la transmiterea matricei.");
				     return errno;
			     }
			     }
		} 
  				
  }	
    }
 }


}
