#include "Enemy.h"
#include "Player.h"
#include "Character.h"

Enemy::Enemy(int h, int w, double positionX, double positionY, double velX, double velY, int hp) : Character(h, w, positionX, positionY, velX, velY, hp)
{
	waitingTimer = (rand() % 75) + 45;
	wanderingTimer = (rand() % 105) + 45;
	wanderDirection = rand() % 4;
	spriteFilePath = "spritesheets/enemy_walking.bmp";
}


Enemy::~Enemy()
{
}

Entity * Enemy::clone() const
{
	return new Enemy(*this);
}

Enemy& Enemy::getPointerToThis()
{
	return *this;
}

void Enemy::determineMovement(double playerPosX, double playerPosY, std::vector<MovementEffect*> effects)
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
	float max;
	if (velocityX < 0.5 && velocityX > -0.5 && velocityY < 0.5 && velocityY > -0.5)
	{
		max = 0.5;
	}
	else
	{
		max = abs(velocityX);
		if (abs(velocityX) < abs(velocityY))
		{
			max = abs(velocityY);
		}
		max *= acceleration;
		if (max > MAX_VELOCITY + maxVelChangeTotal)
		{
			max = MAX_VELOCITY + maxVelChangeTotal;
		}
	}
	if (knockbackTimer == 0)
	{
		if (sqrt(pow(posX - playerPosX, 2) + pow(posY - playerPosY, 2)) < 100)
		{
			float vectorX = playerPosX - getCenterPosX();
			float vectorY = playerPosY - getCenterPosY();
			double magnitude = sqrt(pow(vectorX, 2) + pow(vectorY, 2));
			//get unit vector:
			vectorX /= magnitude;
			vectorY /= magnitude;
			//multiply vector by velocity of enemy:
			vectorX *= max;
			vectorY *= max;

			velocityX = vectorX;
			velocityY = vectorY;
			if (abs(velocityX) > abs(velocityY))
			{
				if (velocityX < 0)
				{
					spriteDirection = 2; //left
				}
				else
				{
					spriteDirection = 1; //right
				}
			}
			else
			{
				if (velocityY < 0)
				{
					spriteDirection = 3; //up
				}
				else
				{
					spriteDirection = 0; //down
				}
			}
		}
		else
		{
			if (velocityX > 0.5 || velocityX < -0.5)
			{
				velocityX *= deceleration;
			}
			else
			{
				velocityX = 0;
			}
			if (velocityY > 0.5 || velocityY < -0.5)
			{
				velocityY *= deceleration;
			}
			else
			{
				velocityY = 0;
			}
			if (wanderingTimer == 0)
			{
				if (waitingTimer == 0)
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
				switch (wanderDirection)
				{
				case 0: //up
					velocityY = max * -1;
					spriteDirection = 3;
					break;
				case 1: //right
					velocityX = max;
					spriteDirection = 1;
					break;
				case 2: //down
					velocityY = max;
					spriteDirection = 0;
					break;
				case 3: //left
					velocityX = max * -1;
					spriteDirection = 2;
					break;
				default:
					break;
				}
				wanderingTimer--;
			}
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

double Enemy::doDamage()
{
	return 1;
}

void Enemy::collideWithEntity(Entity * e)
{
	Player *aPlayer = dynamic_cast<Player*>(e);
	if (aPlayer != nullptr)
	{
		collideWithEntity(aPlayer);
		return;
	}
}

void Enemy::collideWithEntity(Player * p)
{
	if (p->takeDamage(doDamage()))
	{
		knockbackAnEntity(p);
	}
}

void Enemy::hitWall(short direction)
{
	Character::hitWall(direction);
	wanderingTimer = 0; //stop walking into the wall it has just hit
}

