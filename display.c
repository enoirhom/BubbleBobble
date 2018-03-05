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
  glColor3f(0.9f, 0.9f, 0.9f);
  for(int i = 0; i < 500; i++) {
    if (map[i] == '1') {
      drawRect(i%25*20, i/25*20, 20, 20);
    }
  }
}

void displayPlayer(Player player) {
  glColor3f(1.0f, 0.6, 0.2f);
  drawRect(player.x, player.y, player.size, player.size);
}
