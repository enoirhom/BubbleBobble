#include <stdlib.h>
#include <time.h>
#include "bubble.h"

#define BUBBLESLOWDOWN 0.4

// Creae a bubble at the player position
Bubble createBubble(float x, float y, bool isGoingRight) {
	Bubble bubble;
	int randomDuration;

	bubble.x = x;
	bubble.y = y;
	bubble.ySpeed = 0.0;
	bubble.size = 20.0;
	randomDuration = rand() % 75;
	bubble.duration = 550 + randomDuration;

	// Give the bubble a speed depending where the player is looking at
	if(isGoingRight) {
		bubble.xSpeed = 8.0;
	} else {
		bubble.xSpeed = -8.0;
	}

	return bubble;
}

// Calculate the new position of every bubble
void moveBubbles(BubbleNode *bubbleListptr) {
	BubbleNode *element = bubbleListptr->nextBubbleptr;

	while(element != bubbleListptr) {
		Bubble *bubble = &element->data;
		bubble->x += bubble->xSpeed;
		bubble->y += bubble->ySpeed;
		bubble->duration -= 1;

		if(bubble->xSpeed > 0) {
			bubble->xSpeed -= BUBBLESLOWDOWN;
		} else if(bubble->xSpeed < 0) {
			bubble->xSpeed += BUBBLESLOWDOWN;
		}

		if(bubble->xSpeed > -0.5 && bubble->xSpeed < 0.5) {
			bubble->xSpeed = 0.0;
			bubble->ySpeed = 0.2;
		}

		if(bubble->x < 20.0) {
			bubble->x = 20.0;
			bubble->xSpeed = 0.0;
		} else if(bubble->x > 460.0) {
			bubble->x = 460.0;
			bubble->xSpeed = 0.0;
		}

		if(bubble->duration <= 0) {
			element = removeBubbleElement(element);
		} else {
			element = element->nextBubbleptr;
		}
	}
}

/********BUBBLE CHAINED LIST********/

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

void addBubbleElement(BubbleNode *bubbleNodeptr, Bubble bubble) {
	BubbleNode *newNode = malloc(sizeof(BubbleNode));
	newNode->data = bubble;

	bubbleNodeptr->nextBubbleptr->prevBubbleptr = newNode;
	newNode->nextBubbleptr = bubbleNodeptr->nextBubbleptr;
	newNode->prevBubbleptr = bubbleNodeptr;
	bubbleNodeptr->nextBubbleptr = newNode;
}

BubbleNode *removeBubbleElement(BubbleNode *bubbleNodeptr) {
	BubbleNode *prevElemptr, *nextElemptr;
	prevElemptr = bubbleNodeptr->prevBubbleptr;
	nextElemptr = bubbleNodeptr->nextBubbleptr;

	prevElemptr->nextBubbleptr = nextElemptr;
	nextElemptr->prevBubbleptr = prevElemptr;

	free(bubbleNodeptr);

	return nextElemptr;
}

void clearBubbleList(BubbleNode *bubbleNodeptr) {
	BubbleNode *element = bubbleNodeptr->nextBubbleptr;

	while(element != bubbleNodeptr) {
		element = removeBubbleElement(element);
	}
}




