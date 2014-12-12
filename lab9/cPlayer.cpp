#include "cPlayer.h"

cPlayer::cPlayer() : cModel()
{

}

void cPlayer::update(float elapsedTime)
{
	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;

	mdlVelocityAdd.x = -translationX;
	mdlVelocityAdd.y = 0;// -(float)glm::cos(cModel::m_mdlRotation);
	mdlVelocityAdd.z = 0.0f ;

	cModel::m_mdlRotation -= rotationAngle;

	//mdlVelocityAdd *= translationX;
	cModel::m_mdlDirection += mdlVelocityAdd;

	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed *elapsedTime;
	cModel::m_mdlDirection *= 0.95f;
	if (translationX < 0)
	{
		translationX = translationX;
	}
	rotationAngle = 0;
	translationX = 0;
}

cPlayer::~cPlayer()
{

}