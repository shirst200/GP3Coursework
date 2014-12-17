/*
==================================================================================
GameConstants.cpp
==================================================================================
*/

#include "GameConstants.h"

int drawMode = 0;
float rotationAngle = 0;
float currentAngle = 0;
float translationX = 0;
//currentX stores the current X position of the player
float currentX = 0;
//fire is true while the spacebar is pressed down
bool fire = false;
//camera is used to toggle between the camera views
bool camera = true;
//playing toggles wheteher the game is muted or not
bool playing = true;
//moving is used to store the movement state of the player(0=stoped,1=right,2=left)
//This is to enable the player to move and fire at the same time
int moving = 0;
//alive is set to false when the game ends to remove control of the playable character from the player of the game
bool alive = true;
//used to send the instruction to reset the player and enemies
bool restart = false;