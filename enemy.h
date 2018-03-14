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

void moveEnemies(Enemy enemies[], int nbEnemy, char *map);
void findEnemiesDirection(Enemy enemies[], int nbEnemy, Player player);
void enemyIsHit(Enemy *enemy);

#endif
