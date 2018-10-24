#pragma once
#include "Projectile.h"
class Character;
class Arrow :
	public Projectile
{
public:
	Arrow(int h, int w, double positionX, double positionY, double velX, double velY);
	~Arrow();
	Entity* clone() const;
	Arrow& getPointerToThis() override;
	void determineMovement(double playerPosX, double playerPosY, std::vector<TileEffect*> effects) override;
	double doDamage() override;
	void collideWithEntity(Entity * e);
	void collideWithEntity(Character * c);
	void hitWall(short direction) override;
private:
	double previousPosX = -1;
	double previousPosY = -1;
};

