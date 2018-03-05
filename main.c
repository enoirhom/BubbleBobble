#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "display.h"

void display(void);
void computePhysics(int timer);
void changeSize(int w, int h);
void keyPressed(unsigned char key, int x, int y);
void specialKeyPressed(int key, int x, int y);
void specialKeyReleased(int key, int x, int y);

Game *gamePtr;

int main(int argc, char **argv) {
  gamePtr = loadGame();

  // init GLUT and create Window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(500, 400);
  glutCreateWindow("Bubble Bobble");

  // register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(changeSize);
  glutTimerFunc(0, computePhysics, 0);
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeyPressed);
  glutSpecialUpFunc(specialKeyReleased);

  // enter GLUT event processing cycle
  glutMainLoop();

  return 0;
}

// Draws the elements on the screen
void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  displayWalls(gamePtr->map);
  displayPlayer(gamePtr->player);

  glFlush();
}

// Calculate the new position of the elements every 16ms. Ask for a redraw at the end
void computePhysics(int timer) {
  //Recall computePhysics in 16ms
  glutTimerFunc(16, computePhysics, 0);

  movePlayer(&gamePtr->player, gamePtr->map);

  glutPostRedisplay();
}

void changeSize(int w, int h) {
  glViewport (0, 0, (GLsizei)w, (GLsizei)h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho(0, w, 0, h, 0, 100);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

void keyPressed(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
      exit(0);
    default:
      printf("%c\n", key);
  }
}

void specialKeyPressed(int key, int x, int y) {

}


void specialKeyReleased(int key, int x, int y) {

}
