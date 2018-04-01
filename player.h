#ifndef _PLAYER_
#define _PLAYER_

#include <stdbool.h>
#include "bubble.h"

typedef struct {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
  float yMin;
  float size;
  bool isFacingRight;
  int timeSinceHit;
} Player;

void movePlayer(Player *player, BubbleNode *bubbleListptr, char *map);

#endif
