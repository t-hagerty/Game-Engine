#include "Entity.h"



Entity::Entity(int h, int w, double positionX, double positionY, double velX, double velY)
{
	height = h;
	width = w;
	posX = positionX;
	posY = positionY;
	velocityX = velX;
	velocityY = velY;
}

Entity::~Entity()
{
}

void Entity::setPosX(double newX)
{
	posX = newX;
}

double Entity::getPosX() const
{
	return posX;
}

void Entity::setPosY(double newY)
{
	posY = newY;
}

double Entity::getPosY() const
{
	return posY;
}

int Entity::getHeight() const
{
	return height;
}

int Entity::getWidth() const
{
	return width;
}

void Entity::setVelocityX(double newVelX)
{
	velocityX = newVelX;
}

double Entity::getVelocityX() const
{
	return velocityX;
}

void Entity::setVelocityY(double newVelY)
{
	velocityY = newVelY;
}

double Entity::getVelocityY() const
{
	return velocityY;
}
