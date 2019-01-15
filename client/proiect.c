///Client: ReversI

#include "sl.h"
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
#include "grafica.h"

/* codul de eroare returnat de anumite apeluri */
extern int errno;
/* portul de conectare la server*/
int port;

int joaca_joc(struct Client *pclient, struct Grafica *pgrafica)
{

  int KEY = 0;
  char msg[100]; // mesajul trimis
  char a = -1, b = -1;
  char valid;
  int i, j;
  int ok;
  int err;
  int asteapta_tura = 1;
  err = citeste_tura(pclient);
  if (err != 0)
    return err;

  initializeaza_tabla(pclient, pgrafica);
  afiseaza_tabla(pclient, pgrafica);
  if (este_randul_nostru(pclient))
    asteapta_tura = 0;
  slRender();

  while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
  {
    if (!asteapta_tura && verifica_clic_mouse(pgrafica, &a, &b))
    {
      printf("Am citit a=%d b=%d\n", a, b);
      err = valideaza_mutare(pclient, a, b, &valid);
      if (err != 0)
        return err;
      if (valid){
        asteapta_tura = 1;
      }
    }
    if (asteapta_tura){
      err = citeste_tura(pclient);
      if (err != 0)
        return err;
      initializeaza_tabla(pclient, pgrafica);
      afiseaza_tabla(pclient, pgrafica);
    }
    slRender();
  }
  return 0;
}




int main(int argc, char *argv[])
{
  struct Client client;
  struct Grafica grafica;
  int err;

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
  {
    printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
    return -1;
  }

  err = connectare_la_server(&client, argv[1], atoi(argv[2]));
  if (err != 0)
    return err;

  //ne-am conectat, sa vedem care jucator suntem
  citeste_jucator(&client);

  initializeaza_grafica(&grafica);
  //avem jucatorul, hai sa jucam
  err = joaca_joc(&client, &grafica);

  inchide_grafica(&grafica);
  inchide_client(&client);
  return err;
}
