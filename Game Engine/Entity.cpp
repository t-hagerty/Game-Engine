#include "Entity.h"



Entity::Entity(int h, int w, double positionX, double positionY, double velX, double velY)
{
	height = h;
	width = w;
	posX = positionX;
	posY = positionY;
	velocityX = velX;
	velocityY = velY;
	collisionBox = new SDL_Rect();
	collisionBox->x = positionX;
	collisionBox->y = positionY;
	collisionBox->w = w;
	collisionBox->h = h;
}

Entity::~Entity()
{
}

void Entity::setPosX(double newX) const
{
	collisionBox->x = newX;
}

double Entity::getPosX() const
{
	return collisionBox->x;
}

void Entity::setPosY(double newY) const
{
	collisionBox->y = newY;
}

double Entity::getPosY() const
{
	return collisionBox->y;
}

SDL_Rect* Entity::getCollisionBox() const
{
	return collisionBox;
}

int Entity::getHeight() const
{
	return collisionBox->h;
}

int Entity::getWidth() const
{
	return collisionBox->w;
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
