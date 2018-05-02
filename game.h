#ifndef _GAME_
#define _GAME_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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
  GLuint playerSprites[7];
  GLuint enemySprites[8];
  GLuint bubbleSprites[4];
  GLuint wallSprite;
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
void loadTextures(Game *gameptr);

#endif
