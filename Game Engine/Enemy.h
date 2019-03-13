#pragma once
#include "Character.h"
class Player;

class Enemy :
	public Character
{
public:
	Enemy(int h, int w, double positionX, double positionY, double velX, double velY, int hp);
	Enemy(const Enemy &e);
	~Enemy();
	Entity* clone() const;
	Enemy& getPointerToThis() override;
	void determineMovement(double playerPosX, double playerPosY, std::vector<TileEffect*> effects) override;
	double doDamage() override;
	void collideWithEntity(Entity* e);
	void collideWithEntity(Player * p);
	void hitWall(short direction) override;
protected:
	int wanderingTimer;
	int waitingTimer;
	short wanderDirection;
	const float BASE_ACCELERATION = 1.2f;
	const float BASE_DECELERATION = 0.5f;
	float previousVelocity;
};

