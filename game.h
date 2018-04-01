#ifndef _GAME_
#define _GAME_

#include "player.h"
#include "enemy.h"
#include "bubble.h"

#define MAPFILE "map.txt"
#define SAVINGSFILE "savings.dat"
#define MAXSCORELENGTH 9
#define MAXNAMELENGTH 20


typedef struct {
  Player player;
  EnemyNode *enemyListptr;
  BubbleNode *bubbleListptr;
  int score;
  char scoreText[MAXSCORELENGTH];
  int lives;
  char livesText[MAXSCORELENGTH];
  int level;
  char levelText[MAXSCORELENGTH];
  char *map;
} Game;

typedef struct {
	int game;
	int score;
	int lives;
	int level;
} Save;

void newGame(Game **gameptr, int score, int lives, int level);
void addBubble(Game *gameptr);
void checkCollisions(Game *gameptr);
void moveGame(Game *gameptr);
void checkIfEnd(Game *gameptr);
void saveGame(Game *gameptr);
void loadGame(Game **gameptr);
void loadLevel(Game *gameptr);

#endif
