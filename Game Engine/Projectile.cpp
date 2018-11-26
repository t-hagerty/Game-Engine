#include "Projectile.h"



Projectile::Projectile(int entityHeight, int entityWidth, float positionX, float positionY, float velocityX, float velocityY) 
	: Entity(entityHeight, entityWidth, positionX, positionY, velocityX, velocityY)
{
	setHealth(1);
}

Projectile::~Projectile() {}