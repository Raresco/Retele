///Client: ReversI

#include "sl.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#define LUNGIME 796
#define LATIME 596
#define INDICE 74

/* codul de eroare returnat de anumite apeluri */
extern int errno;
char reversiTable[8][8];
/* portul de conectare la server*/
int port;

void initializeaza_tabla(const char *user1,const char *user2,int pat,int lem,int rTable[8][8]){
         //textura tabla
      slSetForeColor(0.1,0.7,0.2,1);
      slSetSpriteScroll(0,0);
      slSprite(pat,500,300,LATIME,LATIME);

                //textura interfata
      slSetSpriteTiling(0.5,0.5);
      slSetSpriteScroll(0.5,0.0);
      slSetForeColor(0.6,0.4,0,1);
      slSprite(lem,100,300,200,600);
      slSetForeColor(0.7,0.5,0.1,1);
      slRectangleOutline(101,300,201,599);
      slSetForeColor(0.6,0.4,0.1,1);
      slRectangleOutline(101,300,200,598);
      slSetForeColor(0.6,0.4,0.1,1);
      slRectangleOutline(101,300,199,597);

                //scrie useri
      slSetForeColor(1,1,1,1);
      slText(20,20,&user1);
      slText(20,565,&user2);
      slRectangleOutline(101,15,200,70);
      slRectangleOutline(101,585,200,70);

                //deseneaza dinii
      for (int i = 204; i <= LUNGIME; i = i + INDICE){
              slSetForeColor(1, 1, 1, 1);
              slLine(i, 4 ,i ,LATIME);
              slSetForeColor(0.9,0.9,0.9,0.8);
              slLine(i+1, 4, i+1, LATIME);
              slLine(i-1, 4, i-1, LATIME);
      }

      for (int i = 4; i <= LATIME; i = i + INDICE){
              slSetForeColor(1, 1, 1, 1);
              slLine(204, i, LUNGIME, i);
              slSetForeColor(0.9,0.9,0.9,0.8);
              slLine(204, i + 1, LUNGIME, i+1);
              slLine(204, i - 1, LUNGIME, i-1);
       }
}



int main (int argc, char *argv[])
{
  int proba=0;
  int sd;  // descriptorul de socket
  int KEY=0;
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100];		// mesajul trimis
  char a=-1,b=-1;
  const int WINDOW_WIDTH = 800;
  const int  WINDOW_HEIGHT = 600;
  char player = 1;
  int i, j, click, click2;
  int pattern, lemn;
  const char* user1;
  const char* user2;
  int ok;
  slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ReversI", 0);
  
  
  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  port = atoi (argv[2]);

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
  char playerNo = -1;
  read(sd, &playerNo, 1);

	//initializare window si background
       // slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ReversI", 0);
        slSetBackColor(0,0.5,0);

        //incarcare texturi
        pattern = slLoadTexture("../client/index.jpeg");
        lemn = slLoadTexture("../client/plp.png");

        //incarcare font
        slSetFont(slLoadFont("../client/font.ttf"),24);

        //incarcare sunet
      	click=slLoadWAV("../client/click.wav");


  while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)){
  char tura = 0;
  ok=0;
  read(sd, &tura, 1);
  printf("Este tura cu numarul %d \n", tura);
  
  for(int i = 0; i < 8; ++i){
          for( int j = 0; j < 8; ++j){
                if(read(sd, &reversiTable[i][j], 1)<=0){
			perror("Nu s-a putut prealua tabla de joc.");
			return errno;
		}
		  printf("%d ", reversiTable[i][j]);
	  }
  	printf("\n");
}
	initializeaza_tabla(user1,user2,pattern,lemn,reversiTable);
              
	for (int i = 0; i < 8; i = i + 1){
              	for (int j = 0; j < 8; j = j + 1){
               	       if (reversiTable[i][j] > 0){
                              slSetForeColor(reversiTable[i][j]-1,reversiTable[i][j]-1,reversiTable[i][j]-1,1);
                              slCircleFill(241 + j*INDICE,INDICE/2+4+(7-i)*INDICE,INDICE/2-3,280);
                              slSetForeColor((reversiTable[i][j]+1)%2,(reversiTable[i][j]+1)%2,(reversiTable[i][j]+1)%2,(reversiTable[i][j]+1)%2);
			      slCircleOutline(241 + j*INDICE,INDICE/2+4+(7-i)*INDICE,INDICE/2-2, 280);
		      }
	      }
	}
slRender();

  if(tura % 2 == playerNo){
  		
	 
  int input=0;
  char valid = -1;
  do{
  //scanf("%d", &a);
  //scanf("%d", &b);
  char mouse = 0;
  do{
            mouse = slGetMouseButton(SL_MOUSE_BUTTON_1);   //doar test
            printf("%d \n", mouse);
	  		slCircleFill(400,400,300,200);
                        a = (slGetMouseX()-204)/INDICE;
                        b = slGetMouseY()/INDICE;
  			input=1;
            printf("%d %d \n", slGetMouseX(),slGetMouseY());
  
  }while(mouse == 0);
if(input==1)
if (write (sd, &a, 1) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
if(input==1)
  if (write (sd, &b, 1) <= 0)
	  perror("[client");
    if(input==1)
     read(sd, &valid, 1);
  printf("%d \n", valid);
  }
  while(valid == 0);
  }  
   for(int i = 0; i < 8; ++i){
          for( int j = 0; j < 8; ++j){
                if(read(sd, &reversiTable[i][j], 1)<=0){
			perror("Nu s-a putut prealua tabla de joc.");
			return errno;
		}
		  printf("%d ", reversiTable[i][j]);
	  }
  	printf("\n");
   }
	for (int i = 0; i < 8; i = i + 1){
                for (int j = 0; j < 8; j = j + 1){
                       if (reversiTable[i][j] > 0){								///Chestia asta am incercat sa o pun intr-o functie si iesea ceva foarte ciudat,
			       											//comenzi cu desene cu aceleasi coordonate desenau chestii unde nu trebuia.am pus-o aici si a mers.
                              slSetForeColor(reversiTable[i][j]-1,reversiTable[i][j]-1,reversiTable[i][j]-1,1);
                              slCircleFill(241 + j*INDICE,INDICE/2+4+(7-i)*INDICE,INDICE/2-3,280);
                              slSetForeColor((reversiTable[i][j]+1)%2,(reversiTable[i][j]+1)%2,(reversiTable[i][j]+1)%2,(reversiTable[i][j]+1)%2);
                              slCircleOutline(241 + j*INDICE,INDICE/2+4+(7-i)*INDICE,INDICE/2-2, 280);
                      }
              }
        }
  slRender();
  
}

  slClose();
  close (sd);
}
