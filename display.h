#ifndef _DISPLAY_
#define _DISPLAY_

#include "game.h"


void displayPlayer(Player player);
void displayEnemies(EnemyNode *enemyListptr);
void displayBubbles(BubbleNode *bubbleListptr);
void displayWalls(char *map);

#endif
