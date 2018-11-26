#include "Player.h"
#include "Enemy.h"

Player::Player(int h, int w, double positionX, double positionY, double velX, double velY, double hp) : Character(h, w, positionX, positionY, velX, velY, hp)
{
	spriteFilePath = "spritesheets/player_walking.bmp";
}

Player::Player(const Player & p) : Character(p.getHeight(), p.getWidth(), p.getPosX(), p.getPosY(), p.getVelocityX(), p.getVelocityY(), p.getHealth())
{
	spriteFilePath = "spritesheets/player_walking.bmp";
}


Player::~Player()
{
	delete collisionBox;
	delete groundHitBox;
	//delete spriteSheet;
}

Entity * Player::clone() const
{
	return new Player(*this);
}

Player & Player::getPointerToThis()
{
	return *this;
}

void Player::determineMovement(double playerPosX, double playerPosY, std::vector<TileEffect*> effects)
{
	float acceleration = BASE_ACCELERATION;
	float deceleration = BASE_DECELERATION;
	float maxVelChangeTotal = 0;

	if (effects.size() != 0)
	{
		for (auto* effect : effects)
		{
			acceleration *= effect->getAccelerationMultiplier();
			deceleration *= effect->getDecelerationMultiplier();
			maxVelChangeTotal += effect->getMaxVelocityChange();
		}
	}

	float max = MAX_VELOCITY + maxVelChangeTotal;
	if (knockbackTimer == 0)
	{
		if (horizontalMovementKeyPress != 0 && verticalMovementKeyPress != 0) //if diagonal movement (will always be 45 degree angle)
		{
			max /= sqrt(2);
		}

		if (horizontalMovementKeyPress == 0)
		{
			setVelocityX(velocityX * deceleration);
			if (velocityX < 0.1 && velocityX > -0.1)
			{
				velocityX = 0;
			}
		}
		else if (horizontalMovementKeyPress == 1)
		{
			if (velocityX < 0.5)
			{
				velocityX = 0.5;
			}
			setVelocityX(velocityX * acceleration);
			if (velocityX > max)
			{
				velocityX = max;
			}
			spriteDirection = 1; //right
		}
		else if (horizontalMovementKeyPress == -1)
		{
			if (velocityX > -0.5)
			{
				velocityX = -0.5;
			}
			setVelocityX(velocityX * acceleration);
			if (velocityX < max * -1)
			{
				velocityX = max * -1;
			}
			spriteDirection = 2; //left
		}
		if (verticalMovementKeyPress == 0)
		{
			setVelocityY(velocityY * deceleration);
			if (velocityY < 0.1 && velocityY > -0.1)
			{
				velocityY = 0;
			}
		}
		else if (verticalMovementKeyPress == 1)
		{
			if (velocityY < 0.5)
			{
				velocityY = 0.5;
			}
			setVelocityY(velocityY * acceleration);
			if (velocityY > max)
			{
				velocityY = max;
			}
			spriteDirection = 0; //down
		}
		else if (verticalMovementKeyPress == -1)
		{
			if (velocityY > -0.5)
			{
				velocityY = -0.5;
			}
			setVelocityY(velocityY * acceleration);
			if (velocityY < max * -1)
			{
				velocityY = max * -1;
			}
			spriteDirection = 3; //up
		}
	}

	if (effects.size() != 0)
	{
		for (auto* effect : effects)
		{
			if (max + abs(effect->getXChange()) > abs(velocityX))
			{
				velocityX += effect->getXChange();
			}
			if (max + abs(effect->getYChange()) > abs(velocityY))
			{
				velocityY += effect->getYChange();
			}
		}
	}
}

void Player::setHorizontalMovementKeyPress(short direction)
{
	if(direction == 0)
	{
		horizontalMovementKeyPress = 0;
		return;
	}
	if(direction > 0)
	{
		horizontalMovementKeyPress = 1;
		return;
	}
	else
	{
		horizontalMovementKeyPress = -1;
	}
}

short Player::getHorizontalMovementKeyPress() const
{
	return horizontalMovementKeyPress;
}

void Player::setVerticalMovementKeyPress(short direction)
{
	if (direction == 0)
	{
		verticalMovementKeyPress = 0;
		return;
	}
	if (direction > 0)
	{
		verticalMovementKeyPress = 1;
		return;
	}
	else
	{
		verticalMovementKeyPress = -1;
	}
}

short Player::getVerticalMovementKeyPress() const
{
	return verticalMovementKeyPress;
}

double Player::doDamage()
{
	return 0.0;
}

void Player::collideWithEntity(Entity * e)
{
	Enemy *anEnemy = dynamic_cast<Enemy*>(e);
	if (anEnemy != nullptr) 
	{
		collideWithEntity(anEnemy);
		return;
	}
}

void Player::collideWithEntity(Enemy * e)
{
	
}


