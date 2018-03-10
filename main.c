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
bool leftKeyPressed = false, rightKeyPressed = false, gamePaused = false;

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
  glutTimerFunc(0, computePhysics, 1);
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
  displayEnemies(gamePtr->enemies, gamePtr->nbEnemy);
  displayPlayer(gamePtr->player);

  glFlush();
}

// Calculate the new position of the elements every 16ms. Ask for a redraw at the end
void computePhysics(int timer) {
  if(timer == 0) {
    //Recall computePhysics in 16ms
    glutTimerFunc(16, computePhysics, 0);

    movePlayer(&gamePtr->player, gamePtr->map);
    moveEnemies(gamePtr->enemies, gamePtr->nbEnemy, gamePtr->map);

    for(int i = 0; i < gamePtr->nbEnemy; i++) {
      if(collidesWithEnemy(gamePtr->player, gamePtr->enemies[i])) {
        gamePtr->enemies[i].isTrapped = true;
      }
    }

    glutPostRedisplay();
  } else if(timer == 1) {
    glutTimerFunc(400, computePhysics, 1);
    findEnemiesDirection(gamePtr->enemies, gamePtr->nbEnemy, gamePtr->player);
  }

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
  switch (key) {
    case GLUT_KEY_UP :
      if(gamePtr->player.y == gamePtr->player.yMin) {
        gamePtr->player.ySpeed = 10.0;
      }
      break;
    case GLUT_KEY_LEFT:
      leftKeyPressed = true;
      gamePtr->player.xSpeed = -5.0;
      break;
    case GLUT_KEY_RIGHT:
      rightKeyPressed = true;
      gamePtr->player.xSpeed = 5.0;
      break;
  }
}


void specialKeyReleased(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      if(rightKeyPressed) {
        gamePtr->player.xSpeed = 5.0;
      } else {
        gamePtr->player.xSpeed = 0.0;
      }
      leftKeyPressed = false;
      break;
    case GLUT_KEY_RIGHT:
      if(leftKeyPressed) {
        gamePtr->player.xSpeed = -5.0;
      } else {
        gamePtr->player.xSpeed = 0.0;
      }
      rightKeyPressed = false;
      break;
  }
}

