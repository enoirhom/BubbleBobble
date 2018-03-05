#include "game.h"
#include <stdlib.h>
#include <stdio.h>

#define MAPFILE "map.txt"


// Create a new Game or load one if the file already exists
Game *loadGame() {
  Game *gamePtr;
  gamePtr = malloc(sizeof(Game));

  gamePtr->player = (Player){.x = 50, .y = 150, .xSpeed = 0, .ySpeed = 0, .size = 20, .isFacingRight = true};
  gamePtr->nbEnemy = 0;
  gamePtr->nbBubble = 0;
  gamePtr->score = 0;
  gamePtr->lives = 3;
  gamePtr->level = 2;
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
