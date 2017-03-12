#pragma once

class Entity
{
public:
	Entity(int h, int w, double positionX, double positionY, double velX, double velY);
	~Entity();
	virtual void test() = 0;
	void setPosX(double newX);
	double getPosX() const;
	void setPosY(double newY);
	double getPosY() const;
	int getHeight() const;
	int getWidth() const;
	void setVelocityX(double newVelX);
	double getVelocityX() const;
	void setVelocityY(double newVelY);
	double getVelocityY() const;
protected:
	int height;
	int width;
	double posX;
	double posY;
	double velocityX; //horizontal distance per frame (per ideal frame, 1/60th of a second, 16.66... milliseconds)
	double velocityY; //vertical distance per frame

};
