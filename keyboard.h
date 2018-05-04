#ifndef _KEYBOARD_
#define _KEYBOARD_

#include "game.h"
#include <stdbool.h>


enum state {
  menu, game, pauseMenu
};

struct keyboard {
	bool leftKeyPressed;    // Is true if the left arrow key is pressed, false if not
	bool rightKeyPressed;   // Is true if the right arrow key is pressed, false if not
};

void gameKeyPressed(unsigned char key, Game *gameptr, enum state *state, int *choice);
void specialGameKeyPressed(int key, Game *gameptr, enum state *state, struct keyboard *keyboard);
void specialGameKeyReleased(int key, Game *gameptr, enum state *state, struct keyboard *keyboard);
void menuKeyPressed(unsigned char key, enum state *state, int choice, Game **gameptr);
void pauseMenuKeyPressed(unsigned char key, enum state *state, int *choice, Game **gameptr);
void specialMenuKeyPressed(int key, int *choice);
void menuMakeChoice(enum state *state, int choice, Game **gameptr);
void pauseMenuMakeChoice(enum state *state, int *choice, Game **gameptr);
void startGame(enum state *state);


#endif
