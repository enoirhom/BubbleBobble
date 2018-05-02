#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "display.h"

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
  if(facingRight) {
    draw(width, height, 1.0, 0.0);
  } else {
    draw(width, height, 0.0, 1.0);
  }

  glEnd();
  glDisable(GL_TEXTURE_2D);
}


/***********GAME DISPLAY***********/

// Draw a rectangle at every '1' in the map array
void displayWalls(char *map, GLuint sprite) {
  glColor3f(1.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, sprite);

  for(int i = 0; i < MAPLENGHT; i++) {
    if (map[i] == '1') {
      drawRect((i % NBMAPCOL) * TILESIZE, i / NBMAPCOL * TILESIZE, TILESIZE, TILESIZE, false);
    }
  }
}

// Draw a rectangle at the player's position
void displayPlayer(Player player) {
  glColor3f(1.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, player.sprites[player.currentSprite/3]);
  
  if(player.timeSinceHit == 0) {
    drawRect(player.x, player.y, player.size, player.size, player.isFacingRight);
  } else {
    if(player.timeSinceHit % 10 == 0) {
      drawRect(player.x, player.y, player.size, player.size, player.isFacingRight);
    }
  }
}

// Draw a recangle at the position of every enemy
void displayEnemies(EnemyNode *enemyListptr) {
  EnemyNode *element = enemyListptr->nextEnemyptr;
  glColor3f(1.0, 1.0, 1.0);

  while(element != enemyListptr) {
    Enemy enemy = element->data;
    if(enemy.isTrapped) {
      glBindTexture(GL_TEXTURE_2D, element->data.sprites[enemy.currentSprite/6]);
    } else {
      glBindTexture(GL_TEXTURE_2D, element->data.sprites[enemy.currentSprite/6]);
    }
    drawRect(enemy.x, enemy.y, enemy.size, enemy.size, enemy.isFacingRight);

    element = element->nextEnemyptr;
  }
}

// Draw a rectangle at the position of every bubble
void displayBubbles(BubbleNode *bubbleListptr) {
  BubbleNode *element = bubbleListptr->nextBubbleptr;

  while(element != bubbleListptr) {
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, element->data.sprites[element->data.currentSprite/10]);
    Bubble bubble = element->data;
    drawRect(bubble.x, bubble.y, bubble.size, bubble.size, false);
    element = element->nextBubbleptr;
  }
}

void displayText(float x, float y, const char *string, void *font) {
  int i = 0;
  
  glLoadIdentity();
  glRasterPos2f(x, y);
  while(string[i] != '\0') {
    glutBitmapCharacter(font, string[i] );
    i++;
  }
}

void displayMenu(char mainMenuText[5][20], int playerChoice) {
  float x = 170.0, y = 300.0;

  glColor3f(0.0, 0.0, 0.0);
  drawRect(20.0, 20.0, 480.0, 380.0, false);
  for(int i = 0; i < 5; i++) {
    if(i == playerChoice) {
      glColor3f(1.0, 0.5, 0.1);
      displayText(x - 12.0, y + 2.0, ">", GLUT_BITMAP_HELVETICA_18);
    } else {
      glColor3f(0.8, 0.8, 0.8);
    }
    displayText(x, y, mainMenuText[i], GLUT_BITMAP_HELVETICA_18);
    y -= 50.0;
  }
}

void displayInterface(Game *gameptr) {
  glColor3f(0.0, 0.0, 0.0);
  displayText(25.0, 5.0, "Score:", GLUT_BITMAP_HELVETICA_18);
  displayText(65.0, 5.0, gameptr->scoreText, GLUT_BITMAP_HELVETICA_18);
  displayText(230.0, 5.0, "Level:", GLUT_BITMAP_HELVETICA_18);
  displayText(270.0, 5.0, gameptr->levelText, GLUT_BITMAP_HELVETICA_18);
  displayText(420.0, 5.0, "Vies:", GLUT_BITMAP_HELVETICA_18);
  displayText(450.0, 5.0, gameptr->livesText, GLUT_BITMAP_HELVETICA_18);
}

void displayGame(Game *gameptr) {
  displayWalls(gameptr->map, gameptr->wallSprite);
  displayEnemies(gameptr->enemyListptr);
  displayBubbles(gameptr->bubbleListptr);
  displayPlayer(gameptr->player);
  displayInterface(gameptr);
}







