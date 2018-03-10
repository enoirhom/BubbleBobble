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
void movePlayer(Player *player, char *map) {
  player->yMin = calculateYMin(*player, map);

  player->x += player->xSpeed;
  player->ySpeed -= 0.5;
  player->y += player->ySpeed;

  if(player->y <= player->yMin) {
    player->y = player->yMin;
    player->ySpeed = 0.0;
  }

  if(player->x < 20.0) {
    player->x = 20.0;
  }

  if(player->x > 460.0) {
    player->x = 460.0;
  }
}

