#include "player.h"
#include <stdio.h>
#include <stdbool.h>


float calculateYMin(Player player, char *map) {
  int row = player.y / 20;
  int col = (player.x + (player.size / 2)) / 20;
  int pos = row * 25 + col;

  for(int i = pos-25; i > 0; i -= 25) {
    if(map[i] == '1') {
      return ((i / 25) * 20.0) + 20.0;
    }
  }
  return 20.0;
}

// Calculate the new position of the player
void movePlayer(Player *playerptr, char *map) {
  playerptr->yMin = calculateYMin(*playerptr, map);

  playerptr->x += playerptr->xSpeed;
  playerptr->ySpeed -= 0.5;
  playerptr->y += playerptr->ySpeed;

  if(playerptr->y <= playerptr->yMin) {
    playerptr->y = playerptr->yMin;
    playerptr->ySpeed = 0.0;
  }

  if(playerptr->x < 20.0) {
    playerptr->x = 20.0;
  }

  if(playerptr->x > 460.0) {
    playerptr->x = 460.0;
  }
}

