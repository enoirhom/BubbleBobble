#include "player.h"
#include <stdio.h>
#include <stdbool.h>

#define NUMBEROFROW 25

// Find the lowest possible y position under the player (platform or bubble)
float calculateYMin(Player player, BubbleNode *bubbleListptr, char *map) {
  float playerMiddle = player.x + (player.size / 2);
  int row = player.y / 20;
  int col = playerMiddle / 20;
  int pos = row * NUMBEROFROW + col;
  int i = pos - NUMBEROFROW;
  float yMin;

  while(i > 0 && map[i] != '1') {
    i -= NUMBEROFROW;
  }

  yMin = ((i / NUMBEROFROW) * 20.0) + 20.0;

  BubbleNode *element = bubbleListptr->nextBubbleptr;

  while(element != bubbleListptr) {
    if(playerMiddle > element->data.x && playerMiddle < (element->data.x + element->data.size)) {
      if(player.y > (element->data.y)) {
        if((element->data.y + element->data.size) > yMin) {
          yMin = element->data.y + element->data.size;
        }
      }
    }
    element = element->nextBubbleptr;
  }

  return yMin;
}

// Calculate the new position of the player
void movePlayer(Player *playerptr, BubbleNode *bubbleListptr, char *map) {
  playerptr->yMin = calculateYMin(*playerptr, bubbleListptr, map);

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

  if(playerptr->timeSinceHit > 0) {
    playerptr->timeSinceHit -= 1;
  }
}

