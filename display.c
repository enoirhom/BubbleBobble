#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "display.h"


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

void displayWalls(char *map) {
  glColor3f(0.9, 0.9, 0.9);
  for(int i = 0; i < 500; i++) {
    if (map[i] == '1') {
      drawRect((i % 25) * 20, i / 25 * 20, 20, 20);
    }
  }
}

void displayPlayer(Player player) {
  glColor3f(0.4, 1.0, 0.2);
  drawRect(player.x, player.y, player.size, player.size);
}

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

void displayBubbles(BubbleNode *bubbleListptr) {
  BubbleNode *element = bubbleListptr->nextBubbleptr;

  glColor3f(0.2, 0.0, 1.0);
  while(element != bubbleListptr) {
    Bubble bubble = element->data;
    drawRect(bubble.x, bubble.y, bubble.size, bubble.size);
    element = element->nextBubbleptr;
  }
}



