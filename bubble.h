#ifndef _BUBBLE_
#define _BUBBLE_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdbool.h>

typedef struct {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
  float size;
  int duration;
  GLuint *sprites;
  int currentSprite;
} Bubble;

struct bubbleNode {
	Bubble data;
	struct bubbleNode *nextBubbleptr;
	struct bubbleNode *prevBubbleptr;
};

typedef struct bubbleNode BubbleNode;



Bubble createBubble(float x, float y, bool isGoingRight, GLuint *sprites);
void moveBubbles(BubbleNode *bubbleListptr);
BubbleNode *newBubbleList(void);
void addBubbleElement(BubbleNode *bubbleNodeptr, Bubble bubble);
BubbleNode *removeBubbleElement(BubbleNode *bubbleNodeptr);
void clearBubbleList(BubbleNode *bubbleNodeptr);

#endif
