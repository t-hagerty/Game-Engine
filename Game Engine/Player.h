#pragma once
#include "Character.h"
class Player :
	public Character
{
public:
	Player(int h, int w, double positionX, double positionY, double velX, double velY, double hp);
	~Player();
	void determineMovement(double playerPosX, double plaerPosY) override;
	void setHorizontalMovementKeyPress(short direction);
	short getHorizontalMovementKeyPress() const;
	void setVerticalMovementKeyPress(short direction);
	short getVerticalMovementKeyPress() const;
private:
	short horizontalMovementKeyPress = 0; // -1 for left, 0 for neither key pressed, 1 for right, should only be one of these three values
	short verticalMovementKeyPress = 0; // -1 for up, 0 for neither key pressed, 1 for down, should only be one of these three values
	const float MAX_VELOCITY = 2;
	const float ACCELERATION = 1.2;
};
