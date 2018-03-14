#ifndef _GAME_
#define _GAME_

#include "player.h"
#include "enemy.h"
#include "bubble.h"


typedef struct {
  Player player;
  EnemyNode *enemyListptr;
  BubbleNode *bubbleListptr;
  int score;
  int lives;
  int level;
  char *map;
} Game;

char *loadMap();
Game *loadGame();
bool playerCollidesWithEnemy(Player player, Enemy enemy);
bool bubbleCollidesWithEnemy(Bubble bubbles, Enemy enemy);
void addBubble(Game *gameptr);


#endif
