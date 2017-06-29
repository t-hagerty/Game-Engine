#include "Enemy.h"



Enemy::Enemy(int h, int w, double positionX, double positionY, double velX, double velY, int hp) : Character(h, w, positionX, positionY, velX, velY, hp)
{
	waitingTimer = (rand() % 75) + 45;
	wanderingTimer = (rand() % 105) + 45;
	wanderDirection = rand() % 4;
}


Enemy::~Enemy()
{
}

void Enemy::determineMovement(double playerPosX, double playerPosY)
{
	if(sqrt(pow(posX - playerPosX, 2) + pow(posY - playerPosY, 2)) < 100)
	{
		float vectorX = playerPosX - posX;
		float vectorY = playerPosY - posY;
		double magnitude = sqrt(pow(vectorX, 2) + pow(vectorY, 2));
		//get unit vector:
		vectorX /= magnitude;
		vectorY /= magnitude;
		//multiply vector by velocity of enemy:
		vectorX *= MAX_VELOCITY;
		vectorY *= MAX_VELOCITY;

		velocityX = vectorX;
		velocityY = vectorY;
	}
	else
	{
		velocityX = 0;
		velocityY = 0;
		if(wanderingTimer == 0)
		{
			if(waitingTimer == 0)
			{
				waitingTimer = (rand() % 75) + 45;
				wanderingTimer = (rand() % 105) + 45;
				wanderDirection = rand() % 4;
				return;
			}
			waitingTimer--;
		}
		else
		{
			switch(wanderDirection)
			{
			case 0: //up
				velocityY = MAX_VELOCITY * -1;
				break;
			case 1: //right
				velocityX = MAX_VELOCITY;
				break;
			case 2: //down
				velocityY = MAX_VELOCITY;
				break;
			case 3: //left
				velocityX = MAX_VELOCITY * -1;
				break;
			default:
				break;
			}
			wanderingTimer--;
		}
	}
}
