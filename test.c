#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>


typedef struct {
  float x;
  float y;
  float size;
  float ySpeed;
  float xSpeed;
} Player;


typedef struct {
  float x;
  float y;
  float width;
  float height;
} Plateform;


int width = 600, height = 600;
Player player;
Plateform ground[2];


void changeSize(int w, int h) {
  width = w;
  height = h;
  ground[0].width = width;
  glViewport (0, 0, (GLsizei)width, (GLsizei)height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho(0, width, 0, height, 0, 100);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

}


void drawRect(float x, float y, float width, float height) {
  glLoadIdentity ();
  glTranslatef(x,y, 0.0f);
  glBegin (GL_POLYGON);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(width, 0.0f);
  glVertex2f(width, height);
  glVertex2i(0.0f, height);
  glEnd();
}


void movePlayer() {
  player.ySpeed -= 0.5f;
  player.y = player.y + player.ySpeed;

  if(player.ySpeed < 0) {
    for(int i = 0; i < 2; i++) {
      if(player.y <= ground[i].y + ground[i].height && player.y >= ground[i].y + ground[i].height - 2.0f) {
        player.y = ground[i].y + ground[i].height;
        player.ySpeed = 0.0f;
      }
    }
  }

  player.x += player.xSpeed;
  if(player.xSpeed < 0.0f) {
    player.xSpeed += 1.0f;
  } else if (player.xSpeed > 0.0f) {
    player.xSpeed -= 1.0f;
  }
}


void physics(int timer) {
  glutTimerFunc(16, physics, ++timer);

  movePlayer();

  glutPostRedisplay();
}


void renderScene(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glColor3f(0.9f, 0.9f, 0.9f);
  drawRect(ground[0].x, ground[0].y, ground[0].width, ground[0].height);
  drawRect(ground[1].x, ground[1].y, ground[1].width, ground[1].height);
  glColor3f(1.0f, 0.6f, 0.0f);
  drawRect(player.x, player.y, player.size, player.size);

  glFlush();
}


void processNormalKeys(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
      exit(0);
    default:
      printf("%c", key);
  }
}


void processSpecialKeys(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_RIGHT:
      player.xSpeed = 12.0f;
      break;
    case GLUT_KEY_LEFT:
      player.xSpeed = -12.0f;
      break;
    case GLUT_KEY_UP:
      player.ySpeed += 10.0f;
      break;
    case GLUT_KEY_DOWN:
      player.y -= 1.0f;
      break;
  }
}


int main(int argc, char **argv) {
  player.x = 100.0f;
  player.y = 200.0f;
  player.size = 20.0f;
  player.ySpeed = 0.0f;
  player.xSpeed = 0.0f;

  ground[0].x = 0.0f;
  ground[0].y = 0.0f;
  ground[0].width = width;
  ground[0].height = 20.0f;

  ground[1].x = 0.0f;
  ground[1].y = 400.0f;
  ground[1].width = width;
  ground[1].height = 20.0f;

  // init GLUT and create Window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(width, height);
  glutCreateWindow("Bubble Bobble");

  // register callbacks
  glutDisplayFunc(renderScene);
  glutReshapeFunc(changeSize);
  glutTimerFunc(0, physics, 0);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);

  // enter GLUT event processing cycle
  glutMainLoop();

  return 0;
}
