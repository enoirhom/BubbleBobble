#ifndef _BUBBLE_
#define _BUBBLE_

#include <stdbool.h>

typedef struct {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
  float size;
  int duration;
} Bubble;

struct bubbleNode {
	Bubble data;
	struct bubbleNode *nextBubbleptr;
	struct bubbleNode *prevBubbleptr;
};

typedef struct bubbleNode BubbleNode;



Bubble createBubble(float x, float y, bool isGoingRight);
void moveBubbles(BubbleNode *bubbleListptr);
BubbleNode *newBubbleList(void);
void addBubbleElement(BubbleNode *bubbleNodeptr, Bubble bubble);
BubbleNode *removeBubbleElement(BubbleNode *bubbleNodeptr);

#endif
