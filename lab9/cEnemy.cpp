#include "cEnemy.h"

cEnemy::cEnemy() : cModel()
{
	
}

void cEnemy::MarchNo(int id)
{
	//The shot time is set to 0, this is the time it first shot so the enemy will not shoot for at least 3 seconds 
	shotTime = 0.0f;
	//the enemy is given 100 health points
	health = 100;
	//The enemy is given it's position accoring to it's id number, this gives the effectg of rows of 5
	//with coloumns starting 1 position to the right of the previous row
	int layer =round( id / 5);
	cModel::m_mdlPosition.y = 0.0f;
	cModel::m_mdlPosition.x = 20 * (id - (5 * layer)) + (5 * layer);
	cModel::m_mdlPosition.z = 40.0f + (10*layer);
	cModel::m_mdlRotation = 0.0f;
	cModel::m_mdlDirection.x = 10;
	cModel::m_mdlDirection.z = 2.0f;
	cModel::m_mdlSpeed = 1.0f;
	cModel::m_IsActive = true;
}
float cEnemy::getShotTime()
{
	//returns the last time this enemy shot
	return shotTime;
}
void cEnemy::setShotTime(float add)
{
	//sets the time tha last shots was fired
	shotTime = add;
}
int cEnemy::getHealth()
{
	//returns the health of this enemy 
	return health;
}
void cEnemy::setHealth(int damage)
{
	//sets the health of this enemy
	health -= damage;
}
int cEnemy::GetSpeed()
{
	//returns the speed of this enemy
	return cModel::m_mdlSpeed;
}
void cEnemy::SetSpeed(int speed)
{
	//sets the speed of this enemy
	cModel::m_mdlSpeed = speed;
}
void cEnemy::update(float elapsedTime)
{
	//moves the enemy according to it's speed
	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
	//checks if the current enemy is at the edge of the playfield and reverses the speed
	if (cModel::m_mdlPosition.x > PLAYFIELDX-50)
		cModel::m_mdlSpeed = -1.0f;
	if (cModel::m_mdlPosition.x < -PLAYFIELDX)
		cModel::m_mdlSpeed = 1.0f;
}

cEnemy::~cEnemy()
{

}

