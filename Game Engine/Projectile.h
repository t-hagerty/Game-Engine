#pragma once
#include "Entity.h"
class Projectile :
	public Entity
{
public:
	Projectile(int entityHeight, int entityWidth, float positionX, float positionY, float velocityX, float velocityY);
	virtual ~Projectile();
	
protected:
};

