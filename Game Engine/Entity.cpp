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
	groundHitBox = new SDL_Rect();
	groundHitBox->x = positionX;
	groundHitBox->y = positionY + (h / 2);
	groundHitBox->w = w;
	groundHitBox->h = h / 2;
}

Entity::~Entity()
{
}

void Entity::setHealth(double newHealth)
{
	health = newHealth;
	if (health < 0)
	{
		health = 0;
	}
}

double Entity::getHealth()
{
	return health;
}

void Entity::setPosX(float newX)
{
	posX = newX;
	collisionBox->x = newX;
	groundHitBox->x = newX;
}

float Entity::getPosX() const
{
	return posX;
}

void Entity::setPosY(float newY)
{
	posY = newY;
	collisionBox->y = newY;
	groundHitBox->y = newY + (height / 2);
}

float Entity::getPosY() const
{
	return posY;
}

SDL_Rect* Entity::getCollisionBox() const
{
	return collisionBox;
}

SDL_Rect * Entity::getGroundHitBox() const
{
	return groundHitBox;
}

float Entity::getCenterPosX()
{
	return getPosX() + (getWidth() / 2);
}

float Entity::getCenterPosY()
{
	return getPosY() + (getHeight() / 2);
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

float Entity::getKnockbackForce()
{
	return KNOCKBACK_FORCE;
}

void Entity::setKnockbackTimer(double time)
{
	knockbackTimer = time;
	if (knockbackTimer < 0)
	{
		knockbackTimer = 0;
	}
}

double Entity::getKnockbackTimer()
{
	return knockbackTimer;
}

void Entity::setInvulnTimer(double time)
{
	invulnTimer = time;
	if (invulnTimer < 0)
	{
		invulnTimer = 0;
	}
}

double Entity::getInvulnTimer()
{
	return invulnTimer;
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

double Entity::takeDamage(double damage)
{
	if (invulnTimer == 0 && damage != 0)
	{
		setHealth(health - damage);
		knockbackTimer = 10; //tied to delta/fps, 60 = 1 second timer
		invulnTimer = 180;
		setAnimationFrame(0);
		return damage;
	}
	else
	{
		return 0;
	}
}

void Entity::knockbackAnEntity(Entity * e)
{
	////Calculate knockback:
	//float vectorX = getCenterPosX() - e->getCenterPosX();
	//float vectorY = getCenterPosY() - e->getCenterPosY();
	//double magnitude = sqrt(pow(vectorX, 2) + pow(vectorY, 2));
	////get unit vector:
	//vectorX /= magnitude;
	//vectorY /= magnitude;
	////multiply vector by velocity of enemy:
	//float knockbackForce = getKnockbackForce() * -1;
	//vectorX *= knockbackForce;
	//vectorY *= knockbackForce;
	//e->setVelocityX(vectorX);
	//e->setVelocityY(vectorY);
	e->beKnockedBack(getCenterPosX(), getCenterPosY(), KNOCKBACK_FORCE);
}

void Entity::beKnockedBack(float knockbackPosX, float knockbackPosY, float knockbackForce)
{
	//Calculate knockback:
	float vectorX = getCenterPosX() - knockbackPosX;
	float vectorY = getCenterPosY() - knockbackPosY;
	double magnitude = sqrt(pow(vectorX, 2) + pow(vectorY, 2));
	//get unit vector:
	vectorX /= magnitude;
	vectorY /= magnitude;
	//multiply vector by velocity of enemy:
	vectorX *= knockbackForce;
	vectorY *= knockbackForce;
	setVelocityX(vectorX);
	setVelocityY(vectorY);
}

void Entity::decrementTimers(double delta)
{
	setKnockbackTimer(knockbackTimer - delta);
	setInvulnTimer(invulnTimer - delta);
}
