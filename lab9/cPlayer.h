#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "GameConstants.h"
#include "cModel.h"
#include "CXBOXController.h"

class cPlayer : public cModel
{
public:
	cPlayer();

	int health;
	int getHealth();
	void setHealth(int damage);

	virtual void update(float elapsedTime);

	~cPlayer();
};
#endif