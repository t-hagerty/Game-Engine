#pragma once
#include <SDL.h>

class Tile
{
public:
	Tile(int x = 0, int y = 0, int size = 0, int typeOfTile = -1, bool isSolid = false);
	~Tile();
	int getType() const;
	bool isSolid() const;
	SDL_Rect getTileSpace() const;
private:
	int type;
	bool solid; //True if impassable by entities
	SDL_Rect tileSpace;
};

