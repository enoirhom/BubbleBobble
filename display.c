#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "display.h"
#include "image_loader.h"

#define MAXSCORELENGTH 9
#define TILESIZE 20
#define MAPLENGHT 500
#define NBMAPCOL 25


void draw(float width, float height, float right, float left) {
  glTexCoord2f(right, 0.0);
  glVertex2f(0, 0);
  glTexCoord2f(left, 0.0);
  glVertex2f(width, 0);
  glTexCoord2f(left, 1.0);
  glVertex2f(width, height);
  glTexCoord2f(right, 1.0);
  glVertex2i(0, height);
}

// Draw a rectangle from the bottom left corner
void drawRect(float x, float y, float width, float height, bool facingRight) {
  glLoadIdentity();
  glTranslatef(x,y, 0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin(GL_POLYGON);
  
  if(facingRight) {     // Flip the texture depending on where the player or enemy is looking
    draw(width, height, 1.0, 0.0);
  } else {
    draw(width, height, 0.0, 1.0);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
}


/*********** GAME DISPLAY ***********/

// Draw a rectangle at every '1' in the map array
void displayWalls(char *map, GLuint sprite) {
  glColor3f(1.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, sprite);     // Change the current texture to wall

  for(int i = 0; i < MAPLENGHT; i++) {
    if (map[i] == '1') {                    // Draw a wall if there is one
      drawRect((i % NBMAPCOL) * TILESIZE, i / NBMAPCOL * TILESIZE, TILESIZE, TILESIZE, false);
    }
  }
}

// Draw a rectangle at the player's position
void displayPlayer(Player player) {
  glColor3f(1.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, player.sprites[player.currentSprite/3]); // Change the current texture to player's current sprite
  
  if(player.timeSinceHit == 0) {                                        // Draw the player if he wasn't hit recently
      drawRect(player.x, player.y, player.size, player.size, player.isFacingRight);
  } else {                                                              // Flash the player if he was hit recently
    if(player.timeSinceHit % 10 == 0) {
      drawRect(player.x, player.y, player.size, player.size, player.isFacingRight);
    }
  }
}

// Draw a recangle at the position of every enemy
void displayEnemies(EnemyNode *enemyListptr) {
  EnemyNode *element = enemyListptr->nextEnemyptr;                                  // Set the element to the first enemyNode in the list
  glColor3f(1.0, 1.0, 1.0);
  
  while(element != enemyListptr) {                                                  // Run through the enemy list
    Enemy enemy = element->data;                                                    // Dereference the enemy at the position in the enemy list
    glBindTexture(GL_TEXTURE_2D, element->data.sprites[enemy.currentSprite/6]);     // Change the current texture to the enemy's current sprite
    drawRect(enemy.x, enemy.y, enemy.size, enemy.size, enemy.isFacingRight);        // Draw the enemy
    
    element = element->nextEnemyptr;                                                // Change the element to the next in the list
  }
}

// Draw a rectangle at the position of every bubble
void displayBubbles(BubbleNode *bubbleListptr) {
  BubbleNode *element = bubbleListptr->nextBubbleptr;                                       // Set the element to the first bubbleNode in the list
  glColor3f(1.0, 1.0, 1.0);
    
  while(element != bubbleListptr) {                                                         // Run through the bubble list
    Bubble bubble = element->data;                                                          // Derefenrece the bubble at the position in the bubble list
    glBindTexture(GL_TEXTURE_2D, element->data.sprites[element->data.currentSprite/10]);    // Change the current texture to the bubble's current sprite
    drawRect(bubble.x, bubble.y, bubble.size, bubble.size, false);                          // Draw the bubble

    element = element->nextBubbleptr;                                                       // Change the element to the next in the list
  }
}


/*********** INTERFACE DISPLAY ***********/

// Display the string parameter at the x and y position
void displayText(float x, float y, const char *string, void *font) {
  int i = 0;
  
  glLoadIdentity();
  glRasterPos2f(x, y);
  while(string[i] != '\0') {                // Run through the string (until the escape character)
    glutBitmapCharacter(font, string[i] );  // Draw each character
    i++;
  }
}

// Display the main and the game pause menu
void displayMenu(char mainMenuText[5][20], int playerChoice) {
  float x = 200.0, y = 265.0;

  glColor3f(0.5, 0.5, 0.5);
  drawRect(20.0, 20.0, 460.0, 360.0, false);    // Draw a background
  for(int i = 0; i < 5; i++) {                  // Display every choice and colorize the current choice
    if(i == playerChoice) {
      glColor3f(1.0, 0.5, 0.1);
      displayText(x - 12.0, y + 2.0, ">", GLUT_BITMAP_HELVETICA_18);
    } else {
      glColor3f(0.8, 0.8, 0.8);
    }
    displayText(x, y, mainMenuText[i], GLUT_BITMAP_HELVETICA_18);
    y -= 25.0;
  }
}

void displayRegles() {
    BMPImage image = loadBMP("./sprites/regles.bmp");
    glRasterPos2f(0.0, 0.0);
    glDrawPixels(image.width, image.height, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    free(image.data);
}

// Display the score, level, and remaining lives
void displayInterface(Game *gameptr) {
  glColor3f(0.0, 0.0, 0.0);
  displayText(25.0, 5.0, "Score:", GLUT_BITMAP_HELVETICA_18);
  displayText(65.0, 5.0, gameptr->scoreText, GLUT_BITMAP_HELVETICA_18);
  displayText(230.0, 5.0, "Level:", GLUT_BITMAP_HELVETICA_18);
  displayText(270.0, 5.0, gameptr->levelText, GLUT_BITMAP_HELVETICA_18);
  displayText(420.0, 5.0, "Live(s):", GLUT_BITMAP_HELVETICA_18);
  displayText(450.0, 5.0, gameptr->livesText, GLUT_BITMAP_HELVETICA_18);
}

void displayGame(Game *gameptr) {
  displayWalls(gameptr->map, gameptr->wallSprite);
  displayEnemies(gameptr->enemyListptr);
  displayBubbles(gameptr->bubbleListptr);
  displayPlayer(gameptr->player);
  displayInterface(gameptr);
}







