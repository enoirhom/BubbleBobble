#include "player.h"
#include <stdio.h>
#include <stdbool.h>

#define NBMAPCOL 25
#define TILESIZE 20
#define GRAVITY 0.5

// Find the lowest possible y position under the player (platform or bubble)
float calculateYMin(Player player, BubbleNode *bubbleListptr, char *map) {
  BubbleNode *element;
  float playerMiddle = player.x + (player.size / 2);
  int row = player.y / TILESIZE;
  int col = playerMiddle / TILESIZE;
  int pos = row * NBMAPCOL + col;
  int i = pos - NBMAPCOL;
  float yMin;

  while(i > 0 && map[i] != '1') {
    i -= NBMAPCOL;
  }

  yMin = (float)((i / NBMAPCOL) * TILESIZE) + TILESIZE;

  element = bubbleListptr->nextBubbleptr;

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

  playerptr->ySpeed -= GRAVITY;
  playerptr->y += playerptr->ySpeed;
  if(playerptr->xSpeed != 0) {
    playerptr->x += playerptr->xSpeed;
    playerptr->currentSprite += 1;
    if(playerptr->currentSprite > 19) {
      playerptr->currentSprite = 0;
    }
  }

  if(playerptr->y <= playerptr->yMin) {
    playerptr->y = playerptr->yMin;
    playerptr->ySpeed = 0.0;
  }

  if(playerptr->x < 20.0) {
    playerptr->x = 20.0;
  } else if(playerptr->x > 460.0) {
    playerptr->x = 460.0;
  }

  if(playerptr->timeSinceHit > 0) {
    playerptr->timeSinceHit -= 1;
  }
}

