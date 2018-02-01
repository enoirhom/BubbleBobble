#include <stdbool.h>

typedef struct {
  int posX;
  int posY;
  int speedX;
  int speedY;
  Hitbox hitbox;
  Pixels sprites[];
} Player;


typedef struct {
  int posX;
  int posY;
  int speedX;
  int speedY;
  Hitbox hitbox;
  Pixels sprites[];
  bool isTraped;
  bool isAlive;
} Enemy;


typedes struct {
  int posX;
  int posY;
  int points;
  int timer;
} Collectable;


int main() {
  Player player;
  Enemy enemies [4];
  Collectable collectable[20];
  int score = 0;
  int lives = 3;
  int level = 1;
}
