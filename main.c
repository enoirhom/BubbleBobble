#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"
#include "display.h"
#include "keyboard.h"
#include "menu.h"


void display(void);
void computePhysics(int timer);
void changeSize(int w, int h);
void keyPressed(unsigned char key, int x, int y);
void specialKeyPressed(int key, int x, int y);
void specialKeyReleased(int key, int x, int y);

char mainMenuText[5][20] = {"Continuer", "Nouvelle partie", "Charger la partie", "Meilleurs scores", "Quitter"};
char pauseMenuText[5][20] = {"Reprendre", "Sauvegarder", "Meilleurs scores", "Menu principal", "Quitter"};
int menuChoice;

enum state state;
Game *gameptr;
struct keyboard keyboard;

int main(int argc, char **argv) {
  srand(time(NULL));

  menuChoice = 1;
  state = menu;
  gameptr = NULL;
  keyboard = (struct keyboard){.leftKeyPressed = false, .rightKeyPressed = false};

  // Init GLUT and create Window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(500, 400);
  glutCreateWindow("Bubble Bobble");
  glutSetCursor(GLUT_CURSOR_NONE);

  // Register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(changeSize);
  glutTimerFunc(0, computePhysics, 0);
  glutTimerFunc(0, computePhysics, 1);
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeyPressed);
  glutSpecialUpFunc(specialKeyReleased);

  // Enter GLUT event processing cycle
  glutMainLoop();

  return 0;
}

// Draws the elements on the screen
void display(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  switch(state) {
    case game:
      displayGame(gameptr);  
      break;    
    case menu:
      displayMenu(mainMenuText, menuChoice);
      break;
    case pauseMenu:
      displayGame(gameptr);
      displayMenu(pauseMenuText, menuChoice);
      break;

  }

  glFlush();
}

// Calculate the new position of the elements every 16ms. Ask for a redraw at the end
void computePhysics(int timer) {
  switch(state) {
    case game:
      if(timer == 0) {
        //Recall computePhysics in 16ms
        glutTimerFunc(16, computePhysics, 0);
        moveGame(gameptr);
        glutPostRedisplay();
      } else if(timer == 1) {
        glutTimerFunc(400, computePhysics, 1);
        findEnemiesDirection(gameptr->enemyListptr, gameptr->player);
      }      
      break;
    case menu:
      glutPostRedisplay();
      break;
    case pauseMenu:
      glutPostRedisplay();
      break;
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
  switch(state) {
    case game:
      gameKeyPressed(key, gameptr, &state, &menuChoice);
      break;
    case pauseMenu:
      pauseMenuKeyPressed(key, &state, &menuChoice, &gameptr);
      break;
    case menu:
      menuKeyPressed(key, &state, menuChoice, &gameptr);
      break;
  }
}

void specialKeyPressed(int key, int x, int y) {
  switch(state) {
    case game:
      specialGameKeyPressed(key, gameptr, &state, &keyboard);
      break;
    case pauseMenu:
      specialMenuKeyPressed(key, &menuChoice);
      break;
    case menu:
      specialMenuKeyPressed(key, &menuChoice);
      break;
  }
}

void specialKeyReleased(int key, int x, int y) {
  switch(state) {
    case game:
      specialGameKeyReleased(key, gameptr, &state, &keyboard);
      break;
    case pauseMenu:
      break;
    case menu:
      break;
  }
}

