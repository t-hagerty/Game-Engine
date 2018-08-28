#pragma once
#include "Character.h"
class Projectile :
	public Character
{
public:
	Projectile(int h, int w, double positionX, double positionY, double velX, double velY, int hp);
	~Projectile();
	Entity* clone() const;
	void determineMovement(double playerPosX, double playerPosY) override;
	double damageCollidedEntity(bool isOtherEntityPlayer) override;
private:
	double previousPosX = -1;
	double previousPosY = -1;
};

