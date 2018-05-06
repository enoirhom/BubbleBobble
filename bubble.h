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
  float x;              // x compound of the bubble position
  float y;              // y compound of the bubble position
  float xSpeed;         // Horizontal speed of the bubble
  float ySpeed;         // Vertical speed of the bubble
  float size;           // Size of the hitbox (square) of the bubble
  int duration;         // Time (in images) the bubble will exist
  GLuint *sprites;      // Pointer to an array of textuteID's
  int currentSprite;    // Number representing the index of the current sprite
} Bubble;

struct bubbleNode {
	Bubble data;
	struct bubbleNode *nextBubbleptr;
	struct bubbleNode *prevBubbleptr;
};

typedef struct bubbleNode BubbleNode;



Bubble createBubble(float x, float y, bool isGoingRight, GLuint *sprites);
void moveBubbles(BubbleNode *bubbleListptr);
BubbleNode *newBubbleList();
void addBubbleElement(BubbleNode *bubbleNodeptr, Bubble bubble);
BubbleNode *removeBubbleElement(BubbleNode *bubbleNodeptr);
void clearBubbleList(BubbleNode *bubbleNodeptr);

#endif
