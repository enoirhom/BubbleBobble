#ifndef _ENEMY_
#define _ENEMY_

#include <stdbool.h>

typedef struct {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
  float size;
  bool isTrapped;
} Enemy;

#endif
