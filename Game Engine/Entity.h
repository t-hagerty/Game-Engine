#pragma once
#include <SDL.h>
#include <string>

class Entity
{
public:
	Entity(int h, int w, float positionX, float positionY, float velX, float velY);
	virtual ~Entity();
	virtual Entity* clone() const = 0;
	void setHealth(double newHealth);
	double getHealth();
	void setPosX(float newX);
	float getPosX() const;
	void setPosY(float newY);
	float getPosY() const;
	SDL_Rect* getCollisionBox() const;
	float getCenterPosX();
	float getCenterPosY();
	int getHeight() const;
	int getWidth() const;
	void setVelocityX(float newVelX);
	float getVelocityX() const;
	void setVelocityY(float newVelY);
	float getVelocityY() const;
	float getKnockbackForce();
	void setKnockbackTimer(double time);
	double getKnockbackTimer();
	void setInvulnTimer(double time);
	double getInvulnTimer();
	std::string getSpriteFilePath() const;
	SDL_Texture* getSpriteSheet() const;
	void setSpriteSheet(SDL_Texture* newSheet);
	int getSpriteWidth() const;
	int getSpriteHeight() const;
	int getSpriteSheetRows() const;
	int getSpriteSheetCols() const;
	int getAnimationFrame() const;
	void setAnimationFrame(int newFrame);
	void incrementAnimationFrame();
	int getSpriteDirection() const;
	void setSpriteDirection(int newDirection);
	virtual void determineMovement(double playerPosX, double playerPosY) = 0;
	double takeDamage(double damage);
	virtual double damageCollidedEntity(bool isOtherEntityPlayer);
	void decrementTimers(double delta);

protected:
	double health;
	int height;
	int width;
	float posX;
	float posY;
	float velocityX; //horizontal distance per frame (per ideal frame, 1/60th of a second, 16.66... milliseconds)
	float velocityY; //vertical distance per frame
	const float MAX_VELOCITY = 1.5;
	const float KNOCKBACK_FORCE = 8;
	double knockbackTimer = 0;
	double invulnTimer = 0;
	SDL_Rect* collisionBox;
	std::string spriteFilePath;
	SDL_Texture* spriteSheet;
	const int SPRITE_WIDTH = 32;
	const int SPRITE_HEIGHT = 32;
	int spriteSheetRows;
	int spriteSheetCols;
	int animationFrame = 0;
	int spriteDirection = 0; //0 = down-facing, 1 = right, 2 = left, 3 = up
};

