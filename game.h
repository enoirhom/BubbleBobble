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


enum state {
    menu, game, pauseMenu, regles
};

typedef struct {
  Player player;                    // Hold the player's information
  EnemyNode *enemyListptr;          // Pointer to the sentinel of enemies chained list
  BubbleNode *bubbleListptr;        // Pointer to the sentinel of bubbles chained list
  int score;                        // Current score of the player
  char scoreText[MAXSCORELENGTH];   // String convertion of the player score, so it can be display in the window
  int lives;                        // Remaining number of lives the player has
  char livesText[MAXSCORELENGTH];   // String convertion of the player remaing lives, so it can be display in the window
  int level;                        // Current level of the game
  char levelText[MAXSCORELENGTH];   // String convertion of the game current level, so it can be display in the window
  char *map;                        // Pointer to an array of char, representing the level's map ('0' = no wall, '1' = wall)
  GLuint playerSprites[7];          // Array of the player textureID's
  GLuint enemySprites[8];           // Array of enemies textureID's
  GLuint bubbleSprites[4];          // Array of bubbles textureID's
  GLuint wallSprite;                // Wall textureID
} Game;

typedef struct {
	int score;  // Score to save
	int lives;  // Remaining lives when saving
	int level;  // Level when saving
} Save;

void newGame(Game **gameptr, int score, int lives, int level);
void addBubble(Game *gameptr);
void checkCollisions(Game *gameptr, enum state *state);
void moveGame(Game *gameptr, enum state *state);
void checkIfEnd(Game *gameptr, enum state *state);
void saveGame(Game *gameptr);
void loadGame(Game **gameptr);
void loadLevel(Game *gameptr);
void loadTextures(Game *gameptr);

#endif
