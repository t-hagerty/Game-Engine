#pragma once
#include <SDL.h>

class Entity
{
public:
	Entity(int h, int w, float positionX, float positionY, float velX, float velY);
	~Entity();
	virtual void test() = 0;
	void setPosX(float newX);
	float getPosX() const;
	void setPosY(float newY);
	float getPosY() const;
	SDL_Rect* getCollisionBox() const;
	int getHeight() const;
	int getWidth() const;
	void setVelocityX(float newVelX);
	float getVelocityX() const;
	void setVelocityY(float newVelY);
	float getVelocityY() const;
protected:
	int height;
	int width;
	float posX;
	float posY;
	float velocityX; //horizontal distance per frame (per ideal frame, 1/60th of a second, 16.66... milliseconds)
	float velocityY; //vertical distance per frame
	SDL_Rect* collisionBox;

};

