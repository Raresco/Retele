#ifndef __GRAFICA_H
#define __GRAFICA_H

#include "client.h"

#define LUNGIME 796
#define LATIME 596
#define INDICE 74
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Grafica
{
    int pattern;
    int lemn;
    int click;
};

void initializeaza_tabla(struct Client *client, struct Grafica *pgrafica);
void initializeaza_grafica(struct Grafica *pgrafica);
void inchide_grafica(struct Grafica *pgrafica);
int afiseaza_tabla(struct Client *pclient, struct Grafica *pgrafica);
int verifica_clic_mouse(struct Grafica *pgrafica, char *a, char *b);
#endif
