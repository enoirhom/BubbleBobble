#ifndef _ENEMY_
#define _ENEMY_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdbool.h>
#include "player.h"


typedef struct {
  float x;              // x compound of the enemy position
  float y;              // y compound of the enemy position
  float xSpeed;         // Horizontal speed of the enemy
  float ySpeed;         // Vertical speed of the enemy
  float yMin;           // The minimum value the y compound of the enemy position can take depending of the current position
  float size;           // Size of the hitbox (square) of the enemy
  bool isFacingRight;   // True if the enemy is looking right, false if he is looking left
  bool isTrapped;       // True if the enemy is trapped
  int timeSinceTrapped; // Time ellapse (in images) since the enemy is trapped
  GLuint *sprites;      // Pointer to an array of textuteID's
  int currentSprite;    // Number representing the index of the current sprite
} Enemy;

struct enemyNode {
  Enemy data;
  struct enemyNode *nextEnemyptr;
  struct enemyNode *prevEnemyptr;
};

typedef struct enemyNode EnemyNode;

void moveEnemies(EnemyNode *enemyListptr, char *map);
void findEnemiesDirection(EnemyNode *enemyListptr, Player player);
void enemyIsHit(Enemy *enemy);
EnemyNode *newEnemyList();
void addEnemyElement(EnemyNode *enemyNodeptr, Enemy enemy);
EnemyNode *removeEnemyElement(EnemyNode *enemyNodeptr);

#endif
