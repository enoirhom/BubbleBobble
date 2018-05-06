#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "image_loader.h"

#define NBMAPCOL 25
#define NBMAPROW 20


void moveGame(Game *gameptr, enum state *state) {
  moveBubbles(gameptr->bubbleListptr);
  movePlayer(&gameptr->player, gameptr->bubbleListptr, gameptr->map);
  moveEnemies(gameptr->enemyListptr, gameptr->map);
  checkCollisions(gameptr, state);
}

void convertIntToString(int score, char scoreText[MAXSCORELENGTH]) {
  int i = 0, j, k = 0;
  char text[MAXSCORELENGTH];
  while(score / 10 != 0) {
    j = score % 10;
    score /= 10;
    text[i] = j + '0';
    i++;
    if(k == 2) {
      text[i] = '.';
      i++;
      k = 0;
    } else {
      k++;
    }
  }
  j = score % 10;
  text[i] = j + '0';
  i++;
  scoreText[i] = '\0';
  j = 0;
  i--;
  while(i >= 0) {
    scoreText[j] = text[i];
    i--;
    j++;
  }
}

// Load the enemies and place the player at the starting point
// Clean the bubbles if any
// Load the map from the textfile depending on the level
// /!\ The map.txt file MUST END WITH '@'
void loadLevel(Game *gameptr) {
  int level = gameptr->level % 7;
  char c;
  FILE *file;
  int i = 0, j = 0, nbEnemy;

  file = fopen(MAPFILE, "r");

  if(file != NULL) {
    if(gameptr->map == NULL) {
      gameptr->map = malloc(NBMAPCOL * NBMAPROW * sizeof(char));
      // Load the textures in graphical memory
      loadTextures(gameptr);
    }

    // Search for the right level. Marked with @level
    fscanf(file, "%c", &c);
    while(c != (level + '0')) {
      while(c != '@') {
        fscanf(file, "%c", &c);
      }
      fscanf(file, "%c", &c);
    }

    // Read the number of enemies
    fscanf(file, "%i", &nbEnemy);

    // Read the enemies position and create them
    while(j < nbEnemy) {
      float x, y;
      fscanf(file, "%f %f", &x, &y);
      addEnemyElement(gameptr->enemyListptr, (Enemy){.x = x, .y = y, .xSpeed = 1.0, .ySpeed = 0.0, .yMin = 0.0, .size = 20.0, .isFacingRight = true, .isTrapped = false, .timeSinceTrapped = 0, .sprites = gameptr->enemySprites, .currentSprite = 0});
      j++;
    }

    // Read the map
    fscanf(file, "%c", &c);
    while(c != '@') {
      if(c != '\n') {
        (gameptr->map)[i] = c;
        i++;
      }
      fscanf(file, "%c", &c);
    }

    fclose(file);

    // Move the player to the starting point
    gameptr->player = (Player){.x = 40.0, .y = 30.0, .xSpeed = 0.0, .ySpeed = 0.0, .yMin = 0.0, .size = 20.0, .isFacingRight = true, .timeSinceHit = 0, .sprites = gameptr->playerSprites, .currentSprite = 0};

    // Remove the bubbles from previous level
    clearBubbleList(gameptr->bubbleListptr);
    
  } else {
    printf("Erreur au chargement du fichier\n");
  }
}

// Create a new Game or load one if the file already exists
void newGame(Game **gameptr, int score, int lives, int level) {
  if(*gameptr == NULL) {
    *gameptr = malloc(sizeof(Game));
  }

  (*gameptr)->enemyListptr = newEnemyList();
  (*gameptr)->bubbleListptr = newBubbleList();
  (*gameptr)->score = score;
  convertIntToString((*gameptr)->score, (*gameptr)->scoreText);
  (*gameptr)->lives = lives;
  convertIntToString((*gameptr)->lives, (*gameptr)->livesText);
  (*gameptr)->level = level;
  convertIntToString((*gameptr)->level, (*gameptr)->levelText);
  (*gameptr)->map = NULL;
  loadLevel(*gameptr);
}

// Save the main information about the game in a binary file
void saveGame(Game *gameptr) {
  FILE *file;
  Save save = (Save){.score = gameptr->score, .lives = gameptr->lives, .level = gameptr->level};


  file = fopen(SAVINGSFILE, "wb");

  if(file != NULL) {
    fwrite(&save, sizeof(Save), 1, file);
  } else {
    printf("File couldn't be opened\n");
  }

  fclose(file);
}

// Load the saved information froom the binary file if already exists
void loadGame(Game **gameptr) {
  FILE *file;
  Save save;

  file = fopen(SAVINGSFILE, "rb");

  if(file != NULL) {
    fread(&save, sizeof(Save), 1, file);
    newGame(gameptr, save.score, save.lives, save.level);
  } else {
    printf("File couldn't be opened\n");
  }

  fclose(file);
}


/******** COLLISION HANDLING *********/

// Check if the player collides with the enemy
bool playerCollidesWithEnemy(Player player, Enemy enemy) {
  return !(player.x > (enemy.x + enemy.size) || (player.x + player.size) < enemy.x || player.y > (enemy.y + enemy.size) || (player.y + player.size) < enemy.y);
}

// Check if the bubble collides with the enemy
bool bubbleCollidesWithEnemy(Bubble bubble, Enemy enemy) {
  return !(bubble.x > (enemy.x + enemy.size) || (bubble.x + bubble.size) < enemy.x || bubble.y > (enemy.y + enemy.size) || (bubble.y + bubble.size) < enemy.y);
}

// Run through every enemy and check if they collide with the player
void checkPlayerCollisions(Game *gameptr, enum state *state) {
  Player *player = &gameptr->player;
  EnemyNode *element = gameptr->enemyListptr->nextEnemyptr;

  // Visit each element of the enemy list, if there is any
  while(element != gameptr->enemyListptr) {
    Enemy *enemy = &element->data;
    // Check the collision
    if(playerCollidesWithEnemy(*player, *enemy)) {
      // If the enemy is trapped when touched, he dies and the score is incremented
      if(enemy->isTrapped) {
        gameptr->score += 100;
        convertIntToString(gameptr->score, gameptr->scoreText);
        element = removeEnemyElement(element);
      // If the enemy isn't trapped, the player loses a live
      } else {
        if(player->timeSinceHit == 0) {
          gameptr->lives -= 1;
          convertIntToString(gameptr->lives, gameptr->livesText);
          player->timeSinceHit = 60;
        }
        element = element->nextEnemyptr;
      }
      checkIfEnd(gameptr, state);
    } else {
      element = element->nextEnemyptr;
    }
  }
}

// Run through every bubble and every enemy and check if they collide
void checkEnemiesCollisions(Game *gameptr) {
  BubbleNode *bubbleElement = gameptr->bubbleListptr->nextBubbleptr;
  EnemyNode *enemyElement = gameptr->enemyListptr->nextEnemyptr;

  // Visit each element of the bubble and enemy list, if there is any
  while(bubbleElement != gameptr->bubbleListptr) {
    Bubble bubble = bubbleElement->data;
    bool bubbleHit = false;
    while(!bubbleHit && enemyElement != gameptr->enemyListptr) {
      Enemy *enemy = &enemyElement->data;
      // Check if there is a collision and if the bubble has enought speed to catch the enemy
      if(bubbleCollidesWithEnemy(bubble, *enemy) && bubble.xSpeed != 0.0) {
        // Check if the enemy isn't already trapped
        if(!enemy->isTrapped){
          enemyIsHit(enemy);
          bubbleHit = true;
        }
      }
      enemyElement = enemyElement->nextEnemyptr;
    }
    // Set the pointer to the first enemy of the list
    enemyElement = gameptr->enemyListptr->nextEnemyptr;

    if(bubbleHit) {
      bubbleElement = removeBubbleElement(bubbleElement);
    } else {
      bubbleElement = bubbleElement->nextBubbleptr;
    }

    bubbleHit = false;
  }
}

void checkCollisions(Game *gameptr, enum state *state) {
  checkPlayerCollisions(gameptr, state);
  checkEnemiesCollisions(gameptr);
}

// Check there are still enemies in the current level. If not load the next level
void checkIfEnd(Game *gameptr, enum state *state) {
  if(gameptr->enemyListptr->nextEnemyptr == gameptr->enemyListptr) {
    gameptr->level += 1;
    convertIntToString(gameptr->level, gameptr->levelText);
    gameptr->score += 1000;
    convertIntToString(gameptr->score, gameptr->scoreText);
    loadLevel(gameptr);
  } else if(gameptr->lives <= 0) {
      *state = menu;
  }
}

// Add a bubble to the bubble list
void addBubble(Game *gameptr) {
  Bubble bubble = createBubble(gameptr->player.x, gameptr->player.y, gameptr->player.isFacingRight, gameptr->bubbleSprites);
  addBubbleElement(gameptr->bubbleListptr, bubble);
}


void loadTextures(Game *gameptr) {
  gameptr->playerSprites[0] = createTexture("./sprites/bob1.bmp");
  gameptr->playerSprites[1] = createTexture("./sprites/bob2.bmp");
  gameptr->playerSprites[2] = createTexture("./sprites/bob3.bmp");
  gameptr->playerSprites[3] = createTexture("./sprites/bob4.bmp");
  gameptr->playerSprites[4] = createTexture("./sprites/bob5.bmp");
  gameptr->playerSprites[5] = createTexture("./sprites/bob6.bmp");
  gameptr->playerSprites[6] = createTexture("./sprites/bob7.bmp");

  gameptr->enemySprites[0] = createTexture("./sprites/enemy1.bmp");
  gameptr->enemySprites[1] = createTexture("./sprites/enemy2.bmp");
  gameptr->enemySprites[2] = createTexture("./sprites/enemy3.bmp");
  gameptr->enemySprites[3] = createTexture("./sprites/enemy4.bmp");
  gameptr->enemySprites[4] = createTexture("./sprites/enemytrapped1.bmp");
  gameptr->enemySprites[5] = createTexture("./sprites/enemytrapped2.bmp");
  gameptr->enemySprites[6] = createTexture("./sprites/enemytrapped3.bmp");
  gameptr->enemySprites[7] = createTexture("./sprites/enemytrapped2.bmp");

  gameptr->bubbleSprites[0] = createTexture("./sprites/bubble1.bmp");
  gameptr->bubbleSprites[1] = createTexture("./sprites/bubble2.bmp");
  gameptr->bubbleSprites[2] = createTexture("./sprites/bubble3.bmp");
  gameptr->bubbleSprites[3] = createTexture("./sprites/bubble2.bmp");

  gameptr->wallSprite = createTexture("./sprites/wall.bmp");
}


