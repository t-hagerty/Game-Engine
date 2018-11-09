#include "Character.h"



Character::Character(int h, int w, double positionX, double positionY, double velX, double velY, double hp) : Entity(h, w, positionX, positionY, velX, velY)
{
	health = hp;
}


Character::~Character()
{
}

void Character::hitWall(short direction)
{
	if (direction == 0 || direction == 3)
	{
		velocityY = 0;
	}
	else if (direction == 1 || direction == 2)
	{
		velocityX = 0;
	}
}


