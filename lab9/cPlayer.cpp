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
	if (alive == true)
	{
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