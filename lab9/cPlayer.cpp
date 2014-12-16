#include "cPlayer.h"

cPlayer::cPlayer() : cModel()
{
	health = 100;
}
int cPlayer::getHealth()
{
	return health;
}
void cPlayer::setHealth(int damage)
{
	health -= damage;
}
void cPlayer::update(float elapsedTime)
{
	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;

	mdlVelocityAdd.x = translationX;
	//mdlVelocityAdd.y = 0;// -(float)glm::cos(cModel::m_mdlRotation);
//mdlVelocityAdd.z = 0.0f ;

	cModel::m_mdlRotation -= rotationAngle;

	cModel::m_mdlDirection += mdlVelocityAdd;

	cModel::m_mdlPosition.x =currentX;
	cModel::m_mdlDirection *= 0.95f;
	rotationAngle = 0;
}

cPlayer::~cPlayer()
{
		
}