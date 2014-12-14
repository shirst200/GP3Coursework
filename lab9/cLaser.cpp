#include "cLaser.h"

cLaser::cLaser() : cModel()
{

}

void cLaser::update(float elapsedTime)
{

	glm::vec3 mdlVelocityAdd;
	mdlVelocityAdd.x = -translationX;
	cModel::m_mdlDirection += mdlVelocityAdd;

	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
	if (cModel::m_mdlPosition.x > PLAYFIELDX ||
		cModel::m_mdlPosition.x < -PLAYFIELDX ||
		cModel::m_mdlPosition.z > PLAYFIELDZ ||
		cModel::m_mdlPosition.z < -PLAYFIELDZ)
		cModel::m_IsActive = false;
}

cLaser::~cLaser()
{

}
