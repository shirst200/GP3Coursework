#ifndef _CENEMIES_H
#define _CENEMIES_H

#include "GameConstants.h"
#include "cModel.h"

class cEnemy : public cModel
{
public:
	cEnemy();

	void MarchNo(int id);
	int GetSpeed();
	void SetSpeed(int speed);
	virtual void update(float elapsedTime);

	~cEnemy();

private:
	const float m_EnemyMinSpeed = 3.0f;
	const float m_EnemyMaxSpeed = 10.0f;
	const float m_EnemySpeedAdjustment = 2.5f;

};
#endif