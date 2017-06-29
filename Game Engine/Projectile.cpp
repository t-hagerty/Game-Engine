#include "Projectile.h"



Projectile::Projectile(int h, int w, double positionX, double positionY, double velX, double velY, int hp) : Character(h, w, positionX, positionY, velX, velY, hp)
{
}


Projectile::~Projectile()
{
}

void Projectile::determineMovement(double playerPosX, double plaerPosY)
{
	if(velocityX == 0 && velocityY == 0)
	{
		delete this; //Projectile no longer moving, hit a wall or another entity
	}
}
