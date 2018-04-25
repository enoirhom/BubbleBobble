#include <stdlib.h>
#include "enemy.h"

#define NBMAPCOL 25
#define TILESIZE 20
#define ENEMYXSPEED 1.0
#define JUMPSPEED 10.0
#define GRAVITY 0.5


// Find the lowest possible y position under the enemy
float enemyCalculateYMin(Enemy enemy, char *map) {
  int row = enemy.y / TILESIZE;
  int col = (enemy.x + (enemy.size / 2)) / TILESIZE;
  int pos = row * NBMAPCOL + col;

  for(int i = pos - NBMAPCOL; i > 0; i -= NBMAPCOL) {
    if(map[i] == '1') {
      return (float)((i / NBMAPCOL) * TILESIZE) + TILESIZE;
    }
  }
  return 20.0;
}

// Change the enemy behavior when hit by a bubble
void enemyIsHit(Enemy *enemyptr) {
  enemyptr->xSpeed = 0.0;
  enemyptr->ySpeed = 0.2;
  enemyptr->isTrapped = true;
  enemyptr->timeSinceTrapped = 300;
}

// Calculate the new position of each enemy
void moveEnemies(EnemyNode *enemyListptr, char *map) {
  EnemyNode *element = enemyListptr->nextEnemyptr;

  // Run throught the enemy list
  while(element != enemyListptr) {
    Enemy *enemy = &element->data;
    if(!enemy->isTrapped) {
      enemy->yMin = enemyCalculateYMin(*enemy, map);

      // Move the enemy depending on its speeds
      enemy->x += enemy->xSpeed;
      enemy->ySpeed -= GRAVITY;
      enemy->y += enemy->ySpeed;
 
      // Stop the enemy from going under platforms
      if(enemy->y <= enemy->yMin) {
        enemy->y = enemy->yMin;
        enemy->ySpeed = 0.0;
      }
    } else {
      enemy->x += enemy->xSpeed;
      enemy->y += enemy->ySpeed;
      enemy->timeSinceTrapped -= 1;

      if(enemy->timeSinceTrapped <= 0) {
        enemy->isTrapped = false;
      }
    }
    element = element->nextEnemyptr;
  }
}

// Calculate a direction depending on the position of the player
void findEnemiesDirection(EnemyNode *enemyListptr, Player player) {
  EnemyNode *element = enemyListptr->nextEnemyptr;

  while(element != enemyListptr) {
    Enemy *enemy = &element->data;
    if(!enemy->isTrapped){
      if(enemy->x < player.x) {
        enemy->xSpeed = ENEMYXSPEED;
      } else {
        enemy->xSpeed = -ENEMYXSPEED;
      }

      if(enemy->y < player.y && enemy->y == enemy->yMin) {
        enemy->ySpeed = JUMPSPEED;
      }
    }

    element = element->nextEnemyptr;
  }
}

/********ENEMY CHAINED LIST********/

EnemyNode *newEnemyList(void) {
  EnemyNode *enemyNodeptr;
  enemyNodeptr = malloc(sizeof(EnemyNode));

  if(enemyNodeptr == NULL) {
    return NULL;
  } else {
    enemyNodeptr->data = (Enemy){.x = 0.0, .y = 0.0, .xSpeed = 0.0, .ySpeed = 0.0, .size = 0.0, .isTrapped = true, .timeSinceTrapped = 0};
    enemyNodeptr->nextEnemyptr = enemyNodeptr;
    enemyNodeptr->prevEnemyptr = enemyNodeptr;
  }

  return enemyNodeptr;
}

void addEnemyElement(EnemyNode *enemyNodeptr, Enemy enemy) {
  EnemyNode *newNode = malloc(sizeof(EnemyNode));
  newNode->data = enemy;

  enemyNodeptr->nextEnemyptr->prevEnemyptr = newNode;
  newNode->nextEnemyptr = enemyNodeptr->nextEnemyptr;
  newNode->prevEnemyptr = enemyNodeptr;
  enemyNodeptr->nextEnemyptr = newNode;
}

EnemyNode *removeEnemyElement(EnemyNode *enemyNodeptr) {
  EnemyNode *prevElemptr, *nextElemptr;
  prevElemptr = enemyNodeptr->prevEnemyptr;
  nextElemptr = enemyNodeptr->nextEnemyptr;

  prevElemptr->nextEnemyptr = nextElemptr;
  nextElemptr->prevEnemyptr = prevElemptr;

  free(enemyNodeptr);

  return nextElemptr;
}
