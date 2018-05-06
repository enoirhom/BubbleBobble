#include <stdlib.h>
#include <time.h>
#include "bubble.h"

#define BUBBLESLOWDOWN 0.4

// Creae a bubble at the player position
Bubble createBubble(float x, float y, bool isGoingRight, GLuint *sprites) {
  Bubble bubble;                            // Create a bubble
  int randomDuration;
    
  bubble.x = x;                             // Set the x position
  bubble.y = y;                             // Set the y position
  bubble.ySpeed = 0.0;                      // Set the vertical speed
  bubble.size = 20.0;                       // Set the hitbox
  randomDuration = rand() % 100;            // Compute the random part of the duration
  bubble.duration = 550 + randomDuration;   // Add the random duration to the minimum duration
  bubble.sprites = sprites;                 // Set the sprite pointer to the array of bubble sprites
  bubble.currentSprite = 0;                 // Set the current sprite to 0

  if(isGoingRight) {                        //
    bubble.xSpeed = 8.0;                    // Give the bubble a speed depending on the player direction
  } else {                                  //
    bubble.xSpeed = -8.0;                   //
  }
    
  return bubble;
}

// Calculate the new position of every bubble
void moveBubbles(BubbleNode *bubbleListptr) {
  BubbleNode *element = bubbleListptr->nextBubbleptr;   // Set the element to the first bubbleNode

  while(element != bubbleListptr) {             // Run through the bubble list
    Bubble *bubble = &element->data;            // Get a pointer to the actual bubble
    bubble->duration -= 1;                      // Decrease the duration by 1 frame

    if(bubble->duration > 0) {
      bubble->currentSprite += 1;               // Go to th next sprite
      
      if(bubble->currentSprite > 39) {          // If there is no more sprites, go back to the first
        bubble->currentSprite = 0;
      }
        
      bubble->x += bubble->xSpeed;              // Move the bubble depending on the x and y speeds
      bubble->y += bubble->ySpeed;
        
      if(bubble->xSpeed > 0.0) {                // If the bubble is still moving horizontally, slow it down
        bubble->xSpeed -= BUBBLESLOWDOWN;
      } else if(bubble->xSpeed < 0.0) {
        bubble->xSpeed += BUBBLESLOWDOWN;
      }
        
      // If the bubble is slow enough, stop it and make it go up
      if(bubble->xSpeed != 0.0 && bubble->xSpeed > -0.5 && bubble->xSpeed < 0.5) {
        bubble->xSpeed = 0.0;
        bubble->ySpeed = 0.2;
      }
      
      if(bubble->x < 20.0) {                    //
        bubble->x = 20.0;                       //
        bubble->xSpeed = 0.0;                   //
      } else if(bubble->x > 460.0) {            // Stop the bubble from leaving the screen
        bubble->x = 460.0;                      //
        bubble->xSpeed = 0.0;                   //
      }
      
    element = element->nextBubbleptr;           // Change the element to the next in the list
    } else {
      element = removeBubbleElement(element);   // Remove the current element of the list and change to the next
    }
  }
}


/******** BUBBLE CHAINED LIST *******/

// Create a new bubble list, create the sentinel and return a pointer to it
BubbleNode *newBubbleList() {
  BubbleNode *bubbleNodeptr;
  bubbleNodeptr = malloc(sizeof(BubbleNode));   // Allow the memomry for the sentinel

  if(bubbleNodeptr == NULL) {                   // If memory allocation worked, set the sentinel to 0 values
    return NULL;
  } else {
    bubbleNodeptr->data = (Bubble){.x = 0.0, .y = 0.0, .xSpeed = 0.0, .ySpeed = 0.0, .size = 0.0, .duration = 0};
    bubbleNodeptr->nextBubbleptr = bubbleNodeptr;
    bubbleNodeptr->prevBubbleptr = bubbleNodeptr;
  }

  return bubbleNodeptr;                         // Return the pointer to the sentinel
}

// Add a bubble element in the list
void addBubbleElement(BubbleNode *bubbleNodeptr, Bubble bubble) {
  BubbleNode *newNode = malloc(sizeof(BubbleNode));         // Allocate memory for the new bubbleNode
  newNode->data = bubble;                                   // Put the bubble in the data of the node

  bubbleNodeptr->nextBubbleptr->prevBubbleptr = newNode;    //
  newNode->nextBubbleptr = bubbleNodeptr->nextBubbleptr;    //
  newNode->prevBubbleptr = bubbleNodeptr;                   // Change the previous pointers
  bubbleNodeptr->nextBubbleptr = newNode;                   //
}

// Remove a bubble element in the list
BubbleNode *removeBubbleElement(BubbleNode *bubbleNodeptr) {
  BubbleNode *prevElemptr, *nextElemptr;            //
  prevElemptr = bubbleNodeptr->prevBubbleptr;       // Saving the previous and next element's pointer
  nextElemptr = bubbleNodeptr->nextBubbleptr;       //

  prevElemptr->nextBubbleptr = nextElemptr;         // Connect previous and next element together
  nextElemptr->prevBubbleptr = prevElemptr;         //
  
  free(bubbleNodeptr);                              // Free the element's memory space

  return nextElemptr;                               // Return a pointer to the next element in the list
}

// Delete every bubble element in the list
void clearBubbleList(BubbleNode *bubbleNodeptr) {
  BubbleNode *element = bubbleNodeptr->nextBubbleptr;   // Set the element to the first bubbleNode in the list

  while(element != bubbleNodeptr) {                     // Run through every bubble element
    element = removeBubbleElement(element);             // Remove the current element
  }
}




