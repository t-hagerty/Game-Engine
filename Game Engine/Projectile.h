#pragma once
#include "Character.h"
class Projectile :
	public Character
{
public:
	Projectile(int h, int w, double positionX, double positionY, double velX, double velY, int hp);
	~Projectile();
	void determineMovement(double playerPosX, double plaerPosY) override;
};

