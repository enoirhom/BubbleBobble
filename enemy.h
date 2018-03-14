#ifndef _ENEMY_
#define _ENEMY_

#include <stdbool.h>
#include "player.h"

typedef struct {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
  float yMin;
  float size;
  bool isTrapped;
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
EnemyNode *newEnemyList(void);
void addEnemyElement(EnemyNode *enemyNodeptr, Enemy enemy);
EnemyNode *removeEnemyElement(EnemyNode *enemyNodeptr);

#endif
