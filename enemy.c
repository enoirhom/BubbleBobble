
#include <stdlib.h>
#include "enemy.h"


float enemyCalculateYMin(Enemy enemy, char *map) {
  int row = enemy.y / 20;
  int col = (enemy.x + (enemy.size / 2)) / 20;
  int pos = row * 25 + col;

  for(int i = pos-25; i > 0; i -= 25) {
    if(map[i] == '1') {
      return ((i / 25) * 20.0) + 20.0;
    }
  }
  return 20.0;
}

void enemyIsHit(Enemy *enemyptr) {
  enemyptr->xSpeed = 0.0;
  enemyptr->ySpeed = 0.2;
  enemyptr->isTrapped = true;
}

void moveEnemies(EnemyNode *enemyListptr, char *map) {
  EnemyNode *element = enemyListptr->nextEnemyptr;

  while(element != enemyListptr) {
    Enemy *enemy = &element->data;
    if(!enemy->isTrapped) {
      enemy->yMin = enemyCalculateYMin(*enemy, map);

      // Move the enemy depending on its speeds
      enemy->x += enemy->xSpeed;
      enemy->ySpeed -= 0.5;
      enemy->y += enemy->ySpeed;
 
      // Stop the enemy from going under platforms
      if(enemy->y <= enemy->yMin) {
        enemy->y = enemy->yMin;
        enemy->ySpeed = 0.0;
      }
    } else {
      enemy->x += enemy->xSpeed;
      enemy->y += enemy->ySpeed;
    }
    element = element->nextEnemyptr;
  }
}

void findEnemiesDirection(EnemyNode *enemyListptr, Player player) {
  EnemyNode *element = enemyListptr->nextEnemyptr;

  while(element != enemyListptr) {
    Enemy *enemy = &element->data;
    if(!enemy->isTrapped){
      if(enemy->x < player.x) {
        enemy->xSpeed = 1.0;
      } else {
        enemy->xSpeed = -1.0;
      }

      if(enemy->y < player.y && enemy->y == enemy->yMin) {
        enemy->ySpeed = 10;
      }
    }

    element = element->nextEnemyptr;
  }
}

EnemyNode *newEnemyList(void) {
  EnemyNode *enemyNodeptr;
  enemyNodeptr = malloc(sizeof(EnemyNode));

  if(enemyNodeptr == NULL) {
    return NULL;
  } else {
    enemyNodeptr->data = (Enemy){.x = 0.0, .y = 0.0, .xSpeed = 0.0, .ySpeed = 0.0, .size = 0.0, .isTrapped = true};
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
