#pragma once
#include "Entity.h"
class Character :
	public Entity
{
public:
	Character(int h, int w, double positionX, double positionY, double velX, double velY, int hp);
	virtual ~Character();
	void hitWall(short direction) override;
protected:

};

