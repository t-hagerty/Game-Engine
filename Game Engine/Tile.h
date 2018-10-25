#pragma once
#include <SDL.h>
#include "TileEffect.h"

class Tile
{
public:
	Tile(int x = 0, int y = 0, int size = 0, int typeOfTile = -1, bool isSolid = false, bool isAPit = false, TileEffect* effect = nullptr);
	~Tile();
	int getType() const;
	bool isSolid() const;
	bool isAPit() const;
	SDL_Rect* getTileSpace() const;
	float getCenterPosX();
	float getCenterPosY();
	TileEffect* getTileEffect();
	void setTileEffect(TileEffect* newEffect);
private:
	int type;
	bool solid; //True if impassable by entities
	bool pit;
	SDL_Rect* tileSpace;
	TileEffect* anEffect;
};

