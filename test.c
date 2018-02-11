#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TILESIZE 20
#define ROW 20
#define COL 25
#define MAXBUBBLE 4

typedef struct {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
} Bubble;

typedef struct {
  float x;
  float y;
  float size;
  float ySpeed;
  float xSpeed;
  Bubble bubbles[MAXBUBBLE];
  int nbBubble;
  bool isFacingRight;
} Player;


int width, height, yMin = 0;
Player player;
//ATTENTION: la map est dessinée à l'envers
int map[ROW][COL] ={{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}};


void changeSize(int w, int h) {
  width = w;
  height = h;
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

int calculateYMin(int playerRow, int playerCol, playerCol2) {
  for(int i = playerRow - 1; i >= 0; i--) {
    if(map[i][playerCol] == 1 || map[i][playerCol2] == 1) {
      return i;
    }
  }
  return -1;
}


void movePlayer() {
  int playerRow, playerCol, playerCol2;
  playerRow = (int) player.y / TILESIZE);
  playerCol = (int) player.x / TILESIZE);
  playerCol2 = (int) (player.x + player.size) / TILESIZE;

  yMin = calculateYMin(playerRow, playerCol, playerCol2) * TILESIZE + TILESIZE;


  player.ySpeed -= 0.5f;
  player.y = player.y + player.ySpeed;

  if(player.y <= yMin) {
    player.y = yMin;
    player.ySpeed = 0.0f;
  }

  player.x += player.xSpeed;
  if(player.xSpeed < 0.0f) {
    player.xSpeed += 1.0f;
  } else if (player.xSpeed > 0.0f) {
    player.xSpeed -= 1.0f;
  }
}


void removeBubble(int n) {
  for(int i = n; i < player.nbBubble - 1; i++) {
    player.bubbles[i] = player.bubbles[i+1];
  }
  player.nbBubble--;
}


void moveBubbles() {
  for(int i = 0; i < player.nbBubble; i++) {
    player.bubbles[i].x += player.bubbles[i].xSpeed;
    if(player.bubbles[i].x > width || player.bubbles[i].x < -TILESIZE) {
      removeBubble(i);
    }
  }
}


void physics(int timer) {
  glutTimerFunc(16, physics, ++timer);

  movePlayer();
  moveBubbles();

  glutPostRedisplay();
}


void renderScene(void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glColor3f(0.9f, 0.9f, 0.9f);
  for(int col = 0; col < COL; col++) {
    for(int row = 0; row < ROW; row++) {
      if(map[row][col] == 1) {
        drawRect(col*TILESIZE, row*TILESIZE, TILESIZE, TILESIZE);
      }
    }
  }

  glColor3f(0.2f, 0.2f, 0.9f);
  for(int i = 0; i < player.nbBubble; i++) {
    drawRect(player.bubbles[i].x, player.bubbles[i].y, TILESIZE, TILESIZE);
  }

  glColor3f(1.0f, 0.6f, 0.0f);
  drawRect(player.x, player.y, player.size, player.size);

  glFlush();
}


Bubble initBubble() {
  Bubble bubble;
  bubble.x = player.x;
  bubble.y = player.y;
  bubble.ySpeed = 0.0f;
  if(player.isFacingRight) {
    bubble.xSpeed = 5.0f;
  } else {
    bubble.xSpeed = -5.0f;
  }
  return bubble;
}


void processNormalKeys(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
      exit(0);
    case 32:
      if(player.nbBubble < MAXBUBBLE) {
        player.bubbles[player.nbBubble] = initBubble();
        player.nbBubble++;
      }
      break;
    default:
      printf("%i", key);
  }
}


void processSpecialKeys(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_RIGHT:
      player.xSpeed = 8.0f;
      player.isFacingRight = true;
      break;
    case GLUT_KEY_LEFT:
      player.xSpeed = -8.0f;
      player.isFacingRight = false;
      break;
    case GLUT_KEY_UP:
      if(player.y == yMin) {
        player.ySpeed += 10.0f;
      }
      break;
    case GLUT_KEY_DOWN:
      player.y -= 1.0f;
      break;
  }
}


int main(int argc, char **argv) {
  width = TILESIZE * COL;
  height = TILESIZE * ROW;
  player.x = 50.0f;
  player.y = 50.0f;
  player.size = 20.0f;
  player.ySpeed = 0.0f;
  player.xSpeed = 0.0f;
  player.isFacingRight = true;
  player.nbBubble = 0;


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
