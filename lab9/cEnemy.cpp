#include "cEnemy.h"

cEnemy::cEnemy() : cModel()
{
	
}

void cEnemy::MarchNo(int id)
{
	shotTime = 0.0f;
	health = 100;
	int layer =round( id / 5);
	cModel::m_mdlPosition.y = 0.0f;
	cModel::m_mdlPosition.x = 10 * (id - (5 * layer)) + (5 * layer);
	cModel::m_mdlPosition.z = 0.0f + (10*layer);   // random number as a float between 0 & 1
	cModel::m_mdlRotation = 0.0f;
	cModel::m_mdlDirection.x = 10;
	cModel::m_mdlDirection.z = 2.0f;
	cModel::m_mdlSpeed = 1.0f;//m_EnemyMinSpeed + rand() / (float)RAND_MAX * m_EnemyMaxSpeed;
	cModel::m_IsActive = true;
}
float cEnemy::getShotTime()
{
	return shotTime;
}
void cEnemy::setShotTime(float add)
{
	shotTime = add;
}
int cEnemy::getHealth()
{
	return health;
}
void cEnemy::setHealth(int damage)
{
	health -= damage;
}
int cEnemy::GetSpeed()
{
	return cModel::m_mdlSpeed;
}
void cEnemy::SetSpeed(int speed)
{
	cModel::m_mdlSpeed = speed;
}
void cEnemy::update(float elapsedTime)
{

	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
	if (cModel::m_mdlPosition.x > PLAYFIELDX-50)
		cModel::m_mdlSpeed = -1.0f;
	if (cModel::m_mdlPosition.x < -PLAYFIELDX)
		cModel::m_mdlSpeed = 1.0f;
	if (cModel::m_mdlPosition.z > PLAYFIELDZ)
		cModel::m_mdlPosition.z -= 2 * PLAYFIELDZ;
	if (cModel::m_mdlPosition.z < -PLAYFIELDZ)
		cModel::m_mdlPosition.z += 2 * PLAYFIELDZ;
	

}

cEnemy::~cEnemy()
{

}

