#ifndef _DISPLAY_
#define _DISPLAY_

#include "game.h"


void displayPlayer(Player player);
void displayEnemies(Enemy enemies[], int nbEnemy);
void displayBubbles(Bubble bubbles[], int nbBubble);
void displayWalls(char *map);

#endif
