#ifndef _GAME_
#define _GAME_

#include "player.h"
#include "enemy.h"
#include "bubble.h"


typedef struct {
  Player player;
  Enemy enemies[10];
  int nbEnemy;
  Bubble bubbles[50];
  int nbBubble;
  int score;
  int lives;
  int level;
  char *map;
} Game;

char *loadMap();
Game *loadGame();


#endif
