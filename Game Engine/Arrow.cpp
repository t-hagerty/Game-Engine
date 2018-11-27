#include "Arrow.h"
#include "Character.h"


Arrow::Arrow(int h, int w, double positionX, double positionY, double velX, double velY) : Projectile(h, w, positionX, positionY, velX, velY)
{
	spriteFilePath = "spritesheets/arrow.bmp";
}

Arrow::Arrow(const Arrow & a) : Projectile(a.getHeight(), a.getWidth(), a.getPosX(), a.getPosY(), a.getVelocityX(), a.getVelocityY())
{
	spriteFilePath = "spritesheets/arrow.bmp";
}


Arrow::~Arrow()
{
	delete collisionBox;
	delete groundHitBox;
	SDL_DestroyTexture(spriteSheet);
}

Entity * Arrow::clone() const
{
	return new Arrow(*this);
}

Arrow & Arrow::getPointerToThis()
{
	return *this;
}

void Arrow::determineMovement(double playerPosX, double playerPosY, std::vector<TileEffect*> effects)
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
