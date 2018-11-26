#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "TileEffect.h"

class Entity
{
public:
	Entity(int h, int w, float positionX, float positionY, float velX, float velY);
	Entity(const Entity &e);
	virtual ~Entity() = 0;
	virtual Entity* clone() const = 0;
	virtual Entity& getPointerToThis() = 0;
	void setHealth(double newHealth);
	double getHealth() const;
	void setPosX(float newX);
	float getPosX() const;
	void setPosY(float newY);
	float getPosY() const;
	SDL_Rect* getCollisionBox() const;
	SDL_Rect* getGroundHitBox() const;
	float getCenterPosX();
	float getCenterPosY();
	float getGroundCenterPosX();
	float getGroundCenterPosY();
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
	virtual void determineMovement(double playerPosX, double playerPosY, std::vector<TileEffect*> effects) = 0;
	double takeDamage(double damage);
	virtual double doDamage() = 0;
	virtual void collideWithEntity(Entity * e) = 0;
	virtual void hitWall(short direction) = 0; //0 = wall down, 1 = wall to the right, 2 = left, 3 = up
	void knockbackAnEntity(Entity * e);
	void beKnockedBack(float knockbackPosX, float knockbackPosY, float knockbackForce);
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
	SDL_Rect* groundHitBox;
	std::string spriteFilePath;
	SDL_Texture* spriteSheet;
	const int SPRITE_WIDTH = 32;
	const int SPRITE_HEIGHT = 32;
	int spriteSheetRows;
	int spriteSheetCols;
	int animationFrame = 0;
	int spriteDirection = 0; //0 = down-facing, 1 = right, 2 = left, 3 = up
};

