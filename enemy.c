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

void moveEnemies(Enemy enemies[], int nbEnemy, char *map) {
  int i = 0;

  while(i < nbEnemy) {
    if(!enemies[i].isTrapped) {
      enemies[i].yMin = enemyCalculateYMin(enemies[i], map);

      // Move the enemy depending on its speeds
      enemies[i].x += enemies[i].xSpeed;
      enemies[i].ySpeed -= 0.5;
      enemies[i].y += enemies[i].ySpeed;
 
      // Stop the enemy from going under platform
      if(enemies[i].y <= enemies[i].yMin) {
        enemies[i].y = enemies[i].yMin;
        enemies[i].ySpeed = 0.0;
      }
    }
    i++;
  }
}

void findEnemiesDirection(Enemy enemies[], int nbEnemy, Player player) {
  int i = 0;

  while(i < nbEnemy) {
    if(enemies[i].x < player.x) {
      enemies[i].xSpeed = 3.0;
    } else {
      enemies[i].xSpeed = -3.0;
    }

    if(enemies[i].y < player.y && enemies[i].y == enemies[i].yMin) {
      enemies[i].ySpeed = 10;
    }

    enemies[i].isTrapped = false;

    i++;
  }
}
