#include "Character.h"



Character::Character(int h, int w, double positionX, double positionY, double velX, double velY, int hp) : Entity(h, w, positionX, positionY, velX, velY)
{
	health = hp;
}


Character::~Character()
{
}
