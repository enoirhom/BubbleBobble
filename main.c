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

Game *gameptr;
bool leftKeyPressed = false, rightKeyPressed = false, gamePaused = false;

int main(int argc, char **argv) {
  gameptr = loadGame();

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

  displayWalls(gameptr->map);
  displayEnemies(gameptr->enemyListptr);
  displayBubbles(gameptr->bubbleListptr);
  displayPlayer(gameptr->player);

  glFlush();
}

// Calculate the new position of the elements every 16ms. Ask for a redraw at the end
void computePhysics(int timer) {
  if(timer == 0) {
    //Recall computePhysics in 16ms
    glutTimerFunc(16, computePhysics, 0);

    movePlayer(&gameptr->player, gameptr->map);
    moveBubbles(gameptr->bubbleListptr);
    moveEnemies(gameptr->enemyListptr, gameptr->map);

    /*
    for(int i = 0; i < gameptr->nbEnemy; i++) {
      if(playerCollidesWithEnemy(gameptr->player, gameptr->enemies[i])) {
        if(gameptr->enemies[i].isTrapped) {
          printf("GAGNE!\n");
        } else {
          gameptr->lives -= 1;
          printf("%i\n", gameptr->lives);
        }
      }
    }

    for(int i = 0; i < gameptr->nbBubble; i++) {
      for(int j = 0; j < gameptr->nbEnemy; j++) {
        if(bubbleCollidesWithEnemy(gameptr->bubbles[i], gameptr->enemies[j])) {
          enemyIsHit(&(gameptr->enemies[j]));
          removeBubble(gameptr->bubbles, &(gameptr->nbBubble), i);
        }
      }
    }
    */

    glutPostRedisplay();
  } else if(timer == 1) {
    glutTimerFunc(400, computePhysics, 1);
    findEnemiesDirection(gameptr->enemyListptr, gameptr->player);
  }

}

void changeSize(int w, int h) {
  glViewport (0, 0, (GLsizei)w, (GLsizei)h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho(0, 500, 0, 400, 0, 100);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

void keyPressed(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
      exit(0);
    case 32:
      addBubble(gameptr);
      break;
    default:
      printf("%i\n", key);
      break;
  }
}

void specialKeyPressed(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP :
      if(gameptr->player.y == gameptr->player.yMin) {
        gameptr->player.ySpeed = 10.0;
      }
      break;
    case GLUT_KEY_LEFT:
      leftKeyPressed = true;
      gameptr->player.xSpeed = -3.0;
      gameptr->player.isFacingRight = false;
      break;
    case GLUT_KEY_RIGHT:
      rightKeyPressed = true;
      gameptr->player.xSpeed = 3.0;
      gameptr->player.isFacingRight = true;
      break;
  }
}


void specialKeyReleased(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      if(rightKeyPressed) {
        gameptr->player.xSpeed = 3.0;
        gameptr->player.isFacingRight = true;
      } else {
        gameptr->player.xSpeed = 0.0;
        gameptr->player.isFacingRight = false;
      }
      leftKeyPressed = false;
      break;
    case GLUT_KEY_RIGHT:
      if(leftKeyPressed) {
        gameptr->player.xSpeed = -3.0;
        gameptr->player.isFacingRight = false;
      } else {
        gameptr->player.xSpeed = 0.0;
        gameptr->player.isFacingRight = true;
      }
      rightKeyPressed = false;
      break;
  }
}

