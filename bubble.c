#include <stdlib.h>
#include "bubble.h"


Bubble creatBubble(float x, float y, bool isGoingRight) {
	Bubble bubble;

	bubble.x = x;
	bubble.y = y;
	bubble.ySpeed = 0.0;
	bubble.size = 20.0;
	bubble.duration = 625;

	if(isGoingRight) {
		bubble.xSpeed = 8.0;
	} else {
		bubble.xSpeed = -8.0;
	}

	return bubble;
}

void removeBubble(Bubble bubbles[], int *nbBubble, int bubble) {
	bubbles[bubble] = bubbles[*nbBubble];
	(*nbBubble) --;
}

void moveBubbles(Bubble bubbles[], int *nbBubble) {
	int i = 0;

	while(i < *nbBubble) {
		Bubble *bubble = &bubbles[i];
		bubble->x += bubble->xSpeed;
		bubble->y += bubble->ySpeed;
		bubbles->duration -= 1;



		if(bubble->xSpeed > 0) {
			bubble->xSpeed -= 0.4;
		} else if(bubbles->xSpeed < 0) {
			bubble->xSpeed += 0.4;
		}

		if(bubble->x < 20.0) {
			bubble->x = 20.0;
			bubble->xSpeed = 0.0;
		} else if(bubble->x > 460.0) {
			bubble->x = 460.0;
			bubble->xSpeed = 0.0;
		}

		if(bubble->duration <= 0) {
			removeBubble(bubbles, nbBubble, i);
		} else {
			i++;
		}
	}
}

BubbleNode *newBubbleList(void) {
	BubbleNode *bubbleNodeptr;
	bubbleNodeptr = malloc(sizeof(BubbleNode));

	if(bubbleNodeptr == NULL) {
		return NULL;
	} else {
		bubbleNodeptr->data = (Bubble){.x = 0.0, .y = 0.0, .xSpeed = 0.0, .ySpeed = 0.0, .size = 0.0, .duration = 0};
		bubbleNodeptr->nextBubbleptr = bubbleNodeptr;
		bubbleNodeptr->prevBubbleptr = bubbleNodeptr;
	}

	return bubbleNodeptr;
}

void addBubbleElement(BubbleNode *bubbleListptr, Bubble bubble) {
	BubbleNode *newNode = malloc(sizeof(BubbleNode));
	newNode->data = bubble;

	bubbleListptr->nextBubbleptr->prevBubbleptr = newNode;
	newNode->nextBubbleptr = bubbleListptr->nextBubbleptr;
	newNode->prevBubbleptr = bubbleListptr;
	bubbleListptr->nextBubbleptr = newNode;
}

void removeBubbleElement(BubbleNode *bubbleNodeptr) {
	BubbleNode *prevElemptr, *nextElemptr;
	prevElemptr = bubbleNodeptr->prevBubbleptr;
	nextElemptr = bubbleNodeptr->nextBubbleptr;

	prevElemptr->nextBubbleptr = nextElemptr;
	nextElemptr->prevBubbleptr = prevElemptr;

	free(bubbleNodeptr);
}






