#ifndef _PLAYER_
#define _PLAYER_

#include <stdbool.h>

typedef struct {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
  float size;
  bool isFacingRight;
} Player;

void movePlayer(Player *player, char *map);

#endif
