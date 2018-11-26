#pragma once
#include "Entity.h"
class Character :
	public Entity
{
public:
	Character(int h, int w, double positionX, double positionY, double velX, double velY, double hp);
	virtual ~Character() = 0;
	void hitWall(short direction) override;
protected:

};

