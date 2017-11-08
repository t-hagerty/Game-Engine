#pragma once
#include "Character.h"
class Enemy :
	public Character
{
public:
	Enemy(int h, int w, double positionX, double positionY, double velX, double velY, int hp);
	~Enemy();
	void determineMovement(double playerPosX, double playerPosY) override;
	double damageCollidedEntity() override;
private:
	int wanderingTimer;
	int waitingTimer;
	short wanderDirection;
};

