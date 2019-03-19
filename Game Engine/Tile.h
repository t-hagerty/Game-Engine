#pragma once
#include <SDL.h>
#include "TileEffect.h"
#include "Entity.h"

class Tile
{
public:
	Tile(int x = 0, int y = 0, int size = 0, int typeOfTile = -1, bool isSolid = false, bool isAPit = false, TileEffect* effect = nullptr);
	Tile(const Tile &t);
	~Tile();
	virtual Tile* clone() const;
	int getType() const;
	bool isSolid() const;
	virtual bool isSolid();
	bool isAPit() const;
	SDL_Rect* getTileSpace() const;
	float getCenterPosX();
	float getCenterPosY();
	int getRow();
	int getCol();
	TileEffect* getTileEffect();
	void setTileEffect(TileEffect* newEffect);
	int getTextureFrameWidth() const;
	int getTextureFrameHeight() const;
	int getTextureFrames() const;
	int getAnimationFrame() const;
	void setAnimationFrame(int newFrame);
	double getRenderAngle() const;
	void setRenderAngle(double newAngle);
	virtual void incrementAnimationFrame();
	virtual void entityEnteredTile(Entity * e);
protected:
	int type;
	bool solid; //True if impassable by entities
	bool pit;
	SDL_Rect* tileSpace;
	TileEffect* anEffect;
	const int TEXTURE_FRAME_WIDTH = 16;
	const int TEXTURE_FRAME_HEIGHT = 16;
	int textureFrames;
	int animationFrame = 0;
	double renderAngle = 0;
	int setNumberFrames() const;
};

