#include "Entity.h"



Entity::Entity(int h, int w, float positionX, float positionY, float velX, float velY)
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

void Entity::setPosX(float newX)
{
	posX = newX;
	collisionBox->x = newX;
}

float Entity::getPosX() const
{
	return posX;
}

void Entity::setPosY(float newY)
{
	posY = newY;
	collisionBox->y = newY;
}

float Entity::getPosY() const
{
	return posY;
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

void Entity::setVelocityX(float newVelX)
{
	velocityX = newVelX;
}

float Entity::getVelocityX() const
{
	return velocityX;
}

void Entity::setVelocityY(float newVelY)
{
	velocityY = newVelY;
}

float Entity::getVelocityY() const
{
	return velocityY;
}
