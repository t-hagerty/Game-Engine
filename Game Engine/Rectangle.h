#pragma once
#include "Entity.h"
class Rectangle :
	public Entity
{
public:
	Rectangle(int h, int w, double positionX, double positionY, double velX, double velY);
	~Rectangle();
	void determineMovement(double playerPosX, double plaerPosY) override;
};

