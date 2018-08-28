#include "Projectile.h"



Projectile::Projectile(int h, int w, double positionX, double positionY, double velX, double velY, int hp) : Character(h, w, positionX, positionY, velX, velY, 2)
{
	spriteFilePath = "spritesheets/arrow.bmp";
}


Projectile::~Projectile()
{
}

Entity * Projectile::clone() const
{
	return new Projectile(*this);
}

void Projectile::determineMovement(double playerPosX, double playerPosY)
{
	if(velocityX == 0 && velocityY == 0)
	{
		setHealth(0);
	}
	if ((previousPosX == posX && velocityX != 0) || (previousPosY == posY && velocityY != 0))
	{
		health = health - 1;
	}
	previousPosX = posX;
	previousPosY = posY;
}

double Projectile::damageCollidedEntity(bool isOtherEntityPlayer)
{
	setHealth(0);
	return 1;
}
