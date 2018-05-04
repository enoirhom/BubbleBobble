#ifndef _PLAYER_
#define _PLAYER_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdbool.h>
#include "bubble.h"

typedef struct {
  float x;              // x compound of the player position
  float y;              // y compound of the player position
  float xSpeed;         // Horizontal speed of the player
  float ySpeed;         // Vertical speed of the player
  float yMin;           // The minimum value the y compound of the player position can take depending of the current position
  float size;           // Size of the hitbox (square) of the player
  bool isFacingRight;   // True if player is looking right, false if he is looking left
  int timeSinceHit;     // Time ellapsed (in images) since the last collision with an enemy
  GLuint *sprites;      // Pointer to an array of TextureID's
  int currentSprite;    // Number representing the index of the current sprite
} Player;

void movePlayer(Player *player, BubbleNode *bubbleListptr, char *map);

#endif
