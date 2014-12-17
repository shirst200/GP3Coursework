#include "cPlayer.h"

cPlayer::cPlayer() : cModel()
{
	//sets the player health(5 units)
	health = 250;
}
int cPlayer::getHealth()
{
	//sends back the player health
	return health;
}
void cPlayer::setHealth(int damage)
{
	//removes damage from health
	health -= damage;
}
void cPlayer::update(float elapsedTime)
{
	CXBOXController* Player1;
	Player1 = new CXBOXController(1);
	if (alive == true)
	{
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT>0) //If the right Arrow key was pressed
		{
			//if the player presses the right key they will move right unitl they lift the key or move left
			moving = 1;
		}
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT>0) //If the Left Arrow key was pressed
		{
			//if the player presses the left key they will move left unitl they lift the key or move right
			moving = 2;
		}
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP>0) //If the UP Arrow key was pressed
		{
			//turns the player right until they releace the button, press another key or reach 45 degrees
			if (currentAngle < 45)
			{
				currentAngle += 5;
				rotationAngle += 5;
			}
		}
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN>0) //If the Down Arrow key was pressed
		{
			//turns the player left until they releace the button, press another key or reach -45 degrees
			if (currentAngle > -45)
			{
				currentAngle -= 5;
				rotationAngle -= 5;
			}
		}
		if (moving == 1 && currentX < PLAYFIELDX)
		{
			//If input right and not at the edge of the game it will move right
			currentX += 0.6f;
			translationX = 0.6f;
		}
		else if (moving == 2 && currentX > -PLAYFIELDX)
		{
			//if input left and not at the edge of the game it will move left
			currentX -= 0.6f;
			translationX = -0.6f;
		}
		// Find out what direction we should be moving using direction.
		glm::vec3 mdlVelocityAdd;
		mdlVelocityAdd.x = translationX;
		//sets the rotation
		cModel::m_mdlRotation -= rotationAngle;

		//moves and rotates the player
		cModel::m_mdlDirection += mdlVelocityAdd;
		cModel::m_mdlPosition.x = currentX;
		cModel::m_mdlDirection *= 0.95f;
		rotationAngle = 0;
	}
}

cPlayer::~cPlayer()
{
	//Stops the player moving and shooting while gives the option to quit or restart
 	alive = false;
}