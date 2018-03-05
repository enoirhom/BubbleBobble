#include "player.h"
#include <stdio.h>


float calculateYMin(Player player, char *map) {
  int row = player.y / 20;
  int col = (player.x + (player.size / 2)) / 20;
  int pos = row * 25 + col;

  for(int i = pos; i > 0; i -= 25) {
    if(map[i] == '1') {
      return ((i / 25) * 20.0) + 20.0;
    }
  }
  return 20.0;
}

// Calculate the new position of the player
void movePlayer(Player *player, char *map) {
  float yMin = calculateYMin(*player, map);

  player->x += player->xSpeed;
  player->ySpeed -= 0.5;
  player->y += player->ySpeed;

  if(player->y <= yMin) {
    player->y = yMin;
    player->ySpeed = 0.0;
  }
}
