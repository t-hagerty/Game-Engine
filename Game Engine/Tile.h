#pragma once
#include <SDL.h>
#include "TileEffect.h"

class Tile
{
public:
	Tile(int x = 0, int y = 0, int size = 0, int typeOfTile = -1, bool isSolid = false, TileEffect* effect = nullptr);
	~Tile();
	int getType() const;
	bool isSolid() const;
	SDL_Rect* getTileSpace() const;
	float getCenterPosX();
	float getCenterPosY();
	TileEffect* getMovementEffect();
	void setMovementEffect(TileEffect* newEffect);
private:
	int type;
	bool solid; //True if impassable by entities
	SDL_Rect* tileSpace;
	TileEffect* anEffect;
};

