#pragma once
#include <SDL.h>
#include "MovementEffect.h"

class Tile
{
public:
	Tile(int x = 0, int y = 0, int size = 0, int typeOfTile = -1, bool isSolid = false, MovementEffect* effect = nullptr);
	~Tile();
	int getType() const;
	bool isSolid() const;
	SDL_Rect getTileSpace() const;
	MovementEffect* getMovementEffect();
	void setMovementEffect(MovementEffect* newEffect);
private:
	int type;
	bool solid; //True if impassable by entities
	SDL_Rect tileSpace;
	MovementEffect* anEffect;
};

