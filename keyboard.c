#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "keyboard.h"
#include "game.h"


void computePhysics(int timer);

// Handle normal key pressures during a game
void gameKeyPressed(unsigned char key, Game *gameptr, enum state *state, int *choice) {
	switch(key) {
		// Pressing "Esc" pauses the game
		case 27:
			*state = pauseMenu;
			*choice = 0; 
			break;
		// Pressing the space bar throws a bubble
		case 32:
			addBubble(gameptr);
			break;
		// Pressing 'A' throws a bubble
		case 97:
			addBubble(gameptr);
			break;
		// Pressing 'Z' allows the player to jump
		case 122:
			// The player can only jump if he already is on a platform or a bubble
			if(gameptr->player.y == gameptr->player.yMin) {
				gameptr->player.ySpeed = 10.0;
			}
			break;
	}
}

// Handle special key pressures during the game
void specialGameKeyPressed(int key, Game *gameptr, enum state *state, struct keyboard *keyboard) {
	switch (key) {
		// Pressing the UP Arrow key allows the player to jump
		case GLUT_KEY_UP:
			if(gameptr->player.y == gameptr->player.yMin) {
				gameptr->player.ySpeed = 10.0;
			}
			break;
		// Pressing the LEFT Arrow key moves the player left (until the key is released)
		case GLUT_KEY_LEFT:
			keyboard->leftKeyPressed = true;
			gameptr->player.xSpeed = -3.0;
			gameptr->player.isFacingRight = false;
			break;
		// Pressing the RIGHT Arrow key moves the player right (until the key is released)
		case GLUT_KEY_RIGHT:
			keyboard->rightKeyPressed = true;
			gameptr->player.xSpeed = 3.0;
			gameptr->player.isFacingRight = true;
			break;
    }
}

// Handle the special key releasing during the game
void specialGameKeyReleased(int key, Game *gameptr, enum state *state, struct keyboard *keyboard) {
	switch (key) {
		// Releasing the LEFT Arrow key stops the player from moving right
		case GLUT_KEY_LEFT:
			// If the RIGHT Arrow key is still pressed, the player moves to the right
			if(keyboard->rightKeyPressed) {
				gameptr->player.xSpeed = 3.0;
				gameptr->player.isFacingRight = true;
			} else {
				gameptr->player.xSpeed = 0.0;
				gameptr->player.isFacingRight = false;
			}
			keyboard->leftKeyPressed = false;
			break;
		// Releasing the TIGHT Arrow key stops the player from moving right
		case GLUT_KEY_RIGHT:
			// If the LEFT Arrow key is still pressed, the player moves to the left
			if(keyboard->leftKeyPressed) {
				gameptr->player.xSpeed = -3.0;
				gameptr->player.isFacingRight = false;
			} else {
				gameptr->player.xSpeed = 0.0;
				gameptr->player.isFacingRight = true;
			}
			keyboard->rightKeyPressed = false;
			break;
	}
}

// Handle normal key pressures while being in the main menu
void menuKeyPressed(unsigned char key, enum state *state, int choice, Game **gameptr) {
	switch(key) {
		// Pressing "Enter" confirms the user choice
		case 13:
			menuMakeChoice(state, choice, gameptr);
			break;
		// Pressing space bar confirms the user choice
		case 32:
			menuMakeChoice(state, choice, gameptr);
			break;
	}
}

// Handle normal key pressures while being in the pause menu
void pauseMenuKeyPressed(unsigned char key, enum state *state, int *choice, Game **gameptr) {
	switch(key) {
		// Pressing "Enter" confirms the user choice
		case 13:
			pauseMenuMakeChoice(state, choice, gameptr);
			break;
		// Pressing space bar confirms the user choice
		case 32:
			pauseMenuMakeChoice(state, choice, gameptr);
			break;
		// Pressing "Esc" unpauses the game
		case 27:
			startGame(state);
			break;
	}
}

// Handle special key pressures while being in the main menu
void specialMenuKeyPressed(int key, int *choice){
	switch(key) {
		// Pressing the UP Arrow key changes the user's choice
		case GLUT_KEY_UP:
			if(*choice > 0) {
				*choice -= 1;
			} else {
				*choice = 4;
			}
			computePhysics(-1);
			break;
		// Pressing the DOWN Arrow key changes the user's choice
		case GLUT_KEY_DOWN:
			if(*choice < 4) {
				*choice += 1;
			} else {
				*choice = 0;
			}
			computePhysics(-1);
			break;
	}
}

// Define what to do depending on the user's choice in the main menu
void menuMakeChoice(enum state *state, int choice, Game **gameptr) {
	switch(choice) {
		// CONTINUER
		case 0:
			if(*gameptr != NULL) {
				startGame(state);
			}
			break;
		// NOUVELLE PARTIE
		case 1:
			newGame(gameptr, 0, 3, 1);
			startGame(state);
			break;
		// CHARGER PARTIE
		case 2:
			loadGame(gameptr);
			break;
		// MEILLEURS SCORES
		case 3:
			break;
		// QUITTER
		case 4: 
			exit(0);
			break;
	}
}

// Define what to do depending on the user's choice in the pause menu
void pauseMenuMakeChoice(enum state *state, int *choice, Game **gameptr) {
	switch(*choice) {
		// REPRENDRE
		case 0:
			startGame(state);
			break;
		// SAUVEGARDER
		case 1:
			saveGame(*gameptr);
			break;
		// MEILLEURS SCORES
		case 2:
			// TO DO
			break;
		// MENU PRINCIPAL
		case 3:
			*state = menu;
			*choice = 0;
			computePhysics(-1);
			break;
		// QUITTER
		case 4: 
			exit(0);
			break;
	}
}

// Restart the game loop
void startGame(enum state *state) {
	*state = game;
	computePhysics(0);
	computePhysics(1);
}




