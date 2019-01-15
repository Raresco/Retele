#include <sl.h>
#include "grafica.h"

void initializeaza_tabla(struct Client *client, struct Grafica *pgrafica)
{
    //textura tabla
    slSetForeColor(0.1, 0.7, 0.2, 1);
    slSetSpriteScroll(0, 0);
    slSprite(pgrafica->pattern, 500, 300, LATIME, LATIME);

    //textura interfata
    slSetSpriteTiling(0.5, 0.5);
    slSetSpriteScroll(0.5, 0.0);
    slSetForeColor(0.6, 0.4, 0, 1);
    slSprite(pgrafica->lemn, 100, 300, 200, 600);
    slSetForeColor(0.7, 0.5, 0.1, 1);
    slRectangleOutline(101, 300, 201, 599);
    slSetForeColor(0.6, 0.4, 0.1, 1);
    slRectangleOutline(101, 300, 200, 598);
    slSetForeColor(0.6, 0.4, 0.1, 1);
    slRectangleOutline(101, 300, 199, 597);

    //scrie useri
    slSetForeColor(1, 1, 1, 1);
    slText(20, 20, client->user1);
    slText(20, 565, client->user2);
    slRectangleOutline(101, 15, 200, 70);
    slRectangleOutline(101, 585, 200, 70);

    //deseneaza dinii
    for (int i = 204; i <= LUNGIME; i = i + INDICE)
    {
        slSetForeColor(1, 1, 1, 1);
        slLine(i, 4, i, LATIME);
        slSetForeColor(0.9, 0.9, 0.9, 0.8);
        slLine(i + 1, 4, i + 1, LATIME);
        slLine(i - 1, 4, i - 1, LATIME);
    }

    for (int i = 4; i <= LATIME; i = i + INDICE)
    {
        slSetForeColor(1, 1, 1, 1);
        slLine(204, i, LUNGIME, i);
        slSetForeColor(0.9, 0.9, 0.9, 0.8);
        slLine(204, i + 1, LUNGIME, i + 1);
        slLine(204, i - 1, LUNGIME, i - 1);
    }
}

void initializeaza_grafica(struct Grafica *pgrafica)
{
    //initializare window si background
    slWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ReversI", 0);
    slSetBackColor(0, 0.5, 0);

    //incarcare texturi
    pgrafica->pattern = slLoadTexture("../client/index.jpeg");
    pgrafica->lemn = slLoadTexture("../client/plp.png");

    //incarcare font
    slSetFont(slLoadFont("../client/font.ttf"), 24);

    //incarcare sunet
    pgrafica->click = slLoadWAV("../client/click.wav");
}

void inchide_grafica(struct Grafica *pgrafica)
{
    slClose();
}

int afiseaza_tabla(struct Client *pclient, struct Grafica *pgrafica)
{
    for (int i = 0; i < 8; i = i + 1)
    {
        for (int j = 0; j < 8; j = j + 1)
        {
            if (pclient->reversiTable[i][j] > 0)
            {
                slSetForeColor(
                    pclient->reversiTable[i][j] - 1,
                    pclient->reversiTable[i][j] - 1,
                    pclient->reversiTable[i][j] - 1, 1);
                slCircleFill(241 + j * INDICE, INDICE / 2 + 4 + (7 - i) * INDICE, INDICE / 2 - 3, 280);
                slSetForeColor(
                    (pclient->reversiTable[i][j] + 1) % 2,
                    (pclient->reversiTable[i][j] + 1) % 2,
                    (pclient->reversiTable[i][j] + 1) % 2,
                    (pclient->reversiTable[i][j] + 1) % 2);
                slCircleOutline(241 + j * INDICE, INDICE / 2 + 4 + (7 - i) * INDICE, INDICE / 2 - 2, 280);
            }
        }
    }
}

int verifica_clic_mouse(struct Grafica *pgrafica, char *a, char *b)
{
    if (slGetMouseButton(SL_MOUSE_BUTTON_1) != 0)
    {
        *a = (slGetMouseX() - 204) / INDICE;
        *b = slGetMouseY() / INDICE;
        return 1;
    }
    return 0;
}
