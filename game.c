#include "game.h"
#include <stdlib.h>
#include <stdio.h>

#define MAPFILE "map.txt"


// Create a new Game or load one if the file already exists
Game *loadGame() {
  Game *gamePtr;
  gamePtr = malloc(sizeof(Game));

  gamePtr->player = (Player){.x = 50.0, .y = 150.0, .xSpeed = 0.0, .ySpeed = 0.0, .yMin = 0.0, .size = 20.0, .isFacingRight = true};
  gamePtr->enemies[0] = (Enemy){.x = 400.0, .y = 300, .xSpeed = 0.0, .ySpeed = 0.0, .yMin = 0.0, .size = 20.0, .isTrapped = false};
  gamePtr->enemies[1] = (Enemy){.x = 160.0, .y = 0.0, .xSpeed = 0.0, .ySpeed = 0.0, .yMin = 0.0, .size = 20.0, .isTrapped = false};
  gamePtr->enemies[2] = (Enemy){.x = 200.0, .y = 300, .xSpeed = 0.0, .ySpeed = 0.0, .yMin = 0.0, .size = 20.0, .isTrapped = false};
  gamePtr->enemies[3] = (Enemy){.x = 230.0, .y = 100, .xSpeed = 0.0, .ySpeed = 0.0, .yMin = 0.0, .size = 20.0, .isTrapped = false};
  gamePtr->nbEnemy = 4;
  gamePtr->nbBubble = 0;
  gamePtr->score = 0;
  gamePtr->lives = 3;
  gamePtr->level = 1;
  gamePtr->map = loadMap(gamePtr->level);

  return gamePtr;
}

// Load the map from a textfile depending on the level
char *loadMap(int level) {
  char *map = NULL, c;
  FILE *file;
  int i = 0;

  file = fopen(MAPFILE, "r");

  if(file != NULL) {
    map = malloc(25 * 20 * sizeof(char));

    // Search for the right level. Marked with @level
    fscanf(file, "%c", &c);
    while(c != (level + 48)) {
      while(c != '@') {
        fscanf(file, "%c", &c);
      }
      fscanf(file, "%c", &c);
    }

    // Read the map
    fscanf(file, "%c", &c);
    while(c != '@') {
      if(c != '\n') {
        map[i] = c;
        i++;
      }
      fscanf(file, "%c", &c);
    }

    fclose(file);
  } else {
    printf("Fichier map pas trouvé\n");
  }

  return map;
}

// Check if the player collides with the enemy
bool collidesWithEnemy(Player player, Enemy enemy) {
  return !(player.x > (enemy.x + enemy.size) || (player.x + player.size) < enemy.x || player.y > (enemy.y + enemy.size) || (player.y + player.size) < enemy.y);
}
