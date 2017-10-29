#include "Player.h"



Player::Player(int h, int w, double positionX, double positionY, double velX, double velY, int hp) : Character(h, w, positionX, positionY, velX, velY, hp)
{
}


Player::~Player()
{
}

void Player::determineMovement(double playerPosX, double plaerPosY)
{
	float max = MAX_VELOCITY;
	if(horizontalMovementKeyPress != 0 && verticalMovementKeyPress != 0) //if diagonal movement (will always be 45 degree angle)
	{
		max /= sqrt(2);
	}

	if(horizontalMovementKeyPress == 0)
	{
		setVelocityX(velocityX / 2);
		if(velocityX < 0.1)
		{
			velocityX = 0;
		}
	}
	else if(horizontalMovementKeyPress == 1)
	{
		if(velocityX < 0.5)
		{
			velocityX = 0.5;
		}
		setVelocityX(velocityX * ACCELERATION);
		if (velocityX > max)
		{
			velocityX = max;
		}
	}
	else if (horizontalMovementKeyPress == -1)
	{
		if (velocityX > -0.5)
		{
			velocityX = -0.5;
		}
		setVelocityX(velocityX * ACCELERATION);
		if (velocityX < max * -1)
		{
			velocityX = max * -1;
		}
	}
	if (verticalMovementKeyPress == 0)
	{
		setVelocityY(velocityY / 2);
		if (velocityY < 0.1)
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
		setVelocityY(velocityY * ACCELERATION);
		if (velocityY > max)
		{
			velocityY = max;
		}
	}
	else if (verticalMovementKeyPress == -1)
	{
		if (velocityY > -0.5)
		{
			velocityY = -0.5;
		}
		setVelocityY(velocityY * ACCELERATION);
		if(velocityY < max * -1)
		{
			velocityY = max * -1;
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