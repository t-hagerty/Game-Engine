#include "Arrow.h"
#include "Character.h"


Arrow::Arrow(int h, int w, double positionX, double positionY, double velX, double velY) : Projectile(h, w, positionX, positionY, velX, velY)
{
	spriteFilePath = "spritesheets/arrow.bmp";
}


Arrow::~Arrow()
{
}

Entity * Arrow::clone() const
{
	return new Arrow(*this);
}

Arrow & Arrow::getPointerToThis()
{
	return *this;
}

void Arrow::determineMovement(double playerPosX, double playerPosY)
{
}

double Arrow::doDamage()
{
	return 1;
}

void Arrow::collideWithEntity(Entity * e)
{
	Character * aCharacter = dynamic_cast<Character*>(e);
	if (aCharacter != nullptr)
	{
		collideWithEntity(aCharacter);
		return;
	}
}

void Arrow::collideWithEntity(Character * c)
{
	printf("Arrow collides with character \n");
	setHealth(0);
	if (c->takeDamage(doDamage()))
	{
		knockbackAnEntity(c);
	}
}

void Arrow::hitWall(short direction)
{
	setHealth(0);
}
