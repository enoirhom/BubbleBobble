#ifndef _DISPLAY_
#define _DISPLAY_

#include "game.h"


void displayGame(Game *gameptr);
void displayText(float x, float y, const char *string, void *font);
void displayMenu(char mainMenuText[5][20], int playerChoice);
void displayRegles();

#endif
