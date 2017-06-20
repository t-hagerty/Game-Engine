#include "Entity.h"



Entity::Entity(int h, int w, float positionX, float positionY, float velX, float velY)
{
	height = h;
	width = w;
	posX = positionX;
	posY = positionY;
	setVelocityX(velX);
	setVelocityY(velY);
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
	if(velocityX > 0)
	{
		spriteDirection = 1;
	}
	else if(velocityX < 0)
	{
		spriteDirection = 2;
	}
}

float Entity::getVelocityX() const
{
	return velocityX;
}

void Entity::setVelocityY(float newVelY)
{
	velocityY = newVelY;
	if(velocityY > 0)
	{
		spriteDirection = 0;
	}
	else if(velocityY < 0)
	{
		spriteDirection = 3;
	}
}

float Entity::getVelocityY() const
{
	return velocityY;
}

std::string Entity::getSpriteFilePath() const
{
	return spriteFilePath;
}

SDL_Texture* Entity::getSpriteSheet() const
{
	return spriteSheet;
}

void Entity::setSpriteSheet(SDL_Texture* newSheet)
{
	SDL_DestroyTexture(spriteSheet);
	spriteSheet = newSheet;
	int sheetWidth, sheetHeight;
	SDL_QueryTexture(spriteSheet, nullptr, nullptr, &sheetWidth, &sheetHeight);
	spriteSheetCols = sheetWidth / SPRITE_WIDTH;
	spriteSheetRows = sheetHeight / SPRITE_HEIGHT;
}

int Entity::getSpriteWidth() const
{
	return SPRITE_WIDTH;
}

int Entity::getSpriteHeight() const
{
	return SPRITE_HEIGHT;
}

int Entity::getSpriteSheetRows() const
{
	return spriteSheetRows;
}

int Entity::getSpriteSheetCols() const
{
	return spriteSheetCols;
}

int Entity::getAnimationFrame() const
{
	return animationFrame;
}

void Entity::setAnimationFrame(int newFrame)
{
	if (newFrame < spriteSheetCols)
	{
		animationFrame = newFrame;
	}
	else
	{
		animationFrame = 0;
	}
}

void Entity::incrementAnimationFrame()
{
	if (animationFrame + 1 < spriteSheetCols)
	{
		animationFrame++;
	}
	else
	{
		animationFrame = 0;
	}
}

int Entity::getSpriteDirection() const
{
	return spriteDirection;
}

void Entity::setSpriteDirection(int newDirection)
{
	if (spriteDirection < 4)
	{
		spriteDirection = newDirection;
	}
}
