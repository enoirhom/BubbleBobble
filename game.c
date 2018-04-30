#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define NBMAPCOL 25
#define NBMAPROW 20


void moveGame(Game *gameptr) {
  moveBubbles(gameptr->bubbleListptr);
  movePlayer(&gameptr->player, gameptr->bubbleListptr, gameptr->map);
  moveEnemies(gameptr->enemyListptr, gameptr->map);
  checkCollisions(gameptr);
  checkIfEnd(gameptr);
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
      addEnemyElement(gameptr->enemyListptr, (Enemy){.x = x, .y = y, .xSpeed = 1.0, .ySpeed = 0.0, .yMin = 0.0, .size = 20.0, .isTrapped = false, .timeSinceTrapped = 0});
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

    // Create the player/ move it to the starting point
    gameptr->player = (Player){.x = 40.0, .y = 30.0, .xSpeed = 0.0, .ySpeed = 0.0, .yMin = 0.0, .size = 20.0, .isFacingRight = true, .timeSinceHit = 0};
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
  Save save = (Save){.game = 1, .score = gameptr->score, .lives = gameptr->lives, .level = gameptr->level};


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


/********COLLISION HANDLING*********/

// Check if the player collides with the enemy
bool playerCollidesWithEnemy(Player player, Enemy enemy) {
  return !(player.x > (enemy.x + enemy.size) || (player.x + player.size) < enemy.x || player.y > (enemy.y + enemy.size) || (player.y + player.size) < enemy.y);
}

// Check if the bubble collides with the enemy
bool bubbleCollidesWithEnemy(Bubble bubble, Enemy enemy) {
  return !(bubble.x > (enemy.x + enemy.size) || (bubble.x + bubble.size) < enemy.x || bubble.y > (enemy.y + enemy.size) || (bubble.y + bubble.size) < enemy.y);
}

// Run through every enemy and check if they collide with the player
void checkPlayerCollisions(Game *gameptr) {
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
      // If the isn't trapped, the player lose a live
      } else {
        if(player->timeSinceHit == 0) {
          gameptr->lives -= 1;
          convertIntToString(gameptr->lives, gameptr->livesText);
          player->timeSinceHit = 60;
          printf("%i\n", gameptr->lives);
        }
        element = element->nextEnemyptr;
      }
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

void checkCollisions(Game *gameptr) {
  checkPlayerCollisions(gameptr);
  checkEnemiesCollisions(gameptr);
}

// Check there are still enemies in the current level. If not load the next level
void checkIfEnd(Game *gameptr) {
  if(gameptr->enemyListptr->nextEnemyptr == gameptr->enemyListptr) {
    gameptr->level += 1;
    convertIntToString(gameptr->level, gameptr->levelText);
    gameptr->score += 1000;
    convertIntToString(gameptr->score, gameptr->scoreText);
    loadLevel(gameptr);
  }
}

// Add a bubble to the bubble list
void addBubble(Game *gameptr) {
  Bubble bubble = createBubble(gameptr->player.x, gameptr->player.y, gameptr->player.isFacingRight);
  addBubbleElement(gameptr->bubbleListptr, bubble);
}


