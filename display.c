#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string.h>
#include <stdio.h>
#include "display.h"

#define MAXSCORELENGTH 9


// Draw a rectangle from the bottom left corner
void drawRect(float x, float y, float width, float height) {
  glLoadIdentity();
  glTranslatef(x,y, 0);
  glBegin(GL_POLYGON);
  glVertex2f(0, 0);
  glVertex2f(width, 0);
  glVertex2f(width, height);
  glVertex2i(0, height);
  glEnd();
}


/***********GAME DISPLAY***********/

// Draw a rectangle at every '1' in the map array
void displayWalls(char *map) {
  glColor3f(0.9, 0.9, 0.9);
  for(int i = 0; i < 500; i++) {
    if (map[i] == '1') {
      drawRect((i % 25) * 20, i / 25 * 20, 20, 20);
    }
  }
}

// Draw a rectangle at the player's position
void displayPlayer(Player player) {
  glColor3f(0.4, 1.0, 0.2);
  drawRect(player.x, player.y, player.size, player.size);
}

// Draw a recangle at the position of every enemy
void displayEnemies(EnemyNode *enemyListptr) {
  EnemyNode *element = enemyListptr->nextEnemyptr;

  while(element != enemyListptr) {
    Enemy enemy = element->data;
    if(enemy.isTrapped) {
      glColor3f(0.6, 0.0, 0.6);
    } else {
      glColor3f(1.0, 0.0, 0.2);
    }
    drawRect(enemy.x, enemy.y, enemy.size, enemy.size);

    element = element->nextEnemyptr;
  }
}

// Draw a rectangle at the position of every bubble
void displayBubbles(BubbleNode *bubbleListptr) {
  BubbleNode *element = bubbleListptr->nextBubbleptr;

  while(element != bubbleListptr) {
    Bubble bubble = element->data;
    if(bubble.duration > 60) {
      glColor3f(0.2, 0.0, 1.0);
    } else {
      glColor3f(0.8, 0.5, 0.95);
    }
    drawRect(bubble.x, bubble.y, bubble.size, bubble.size);
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
  drawRect(160.0, 90.0, 180.0, 230.0);
  for(int i = 0; i < 5; i++) {
    if(i == playerChoice) {
      glColor3f(1.0, 0.5, 0.1);
    } else {
      glColor3f(0.8, 0.8, 0.8);
    }
    displayText(x, y, mainMenuText[i], GLUT_BITMAP_HELVETICA_18);
    y -= 50.0;
  }
}

void displayInterface(Game *gameptr) {
  glColor3f(0.0, 0.0, 0.0);
  displayText(25.0, 5.0, "Score:", GLUT_BITMAP_HELVETICA_12);
  displayText(65.0, 5.0, gameptr->scoreText, GLUT_BITMAP_HELVETICA_12);
  displayText(230.0, 5.0, "Level:", GLUT_BITMAP_HELVETICA_12);
  displayText(270.0, 5.0, gameptr->levelText, GLUT_BITMAP_HELVETICA_12);
  displayText(420.0, 5.0, "Vies:", GLUT_BITMAP_HELVETICA_12);
  displayText(450.0, 5.0, gameptr->livesText, GLUT_BITMAP_HELVETICA_12);
}

void displayGame(Game *gameptr) {
  displayWalls(gameptr->map);
  displayEnemies(gameptr->enemyListptr);
  displayBubbles(gameptr->bubbleListptr);
  displayPlayer(gameptr->player);
  displayInterface(gameptr);
}
