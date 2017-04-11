#include "Tile.h"



Tile::Tile(int x, int y, int size, int typeOfTile, bool isSolid)
{
	tileSpace = { 0, 0, 0, 0 };
	tileSpace.x = x;
	tileSpace.y = y;
	tileSpace.w = size;
	tileSpace.h = size;
	type = typeOfTile;
	solid = isSolid;
}


Tile::~Tile()
{
}

int Tile::getType() const
{
	return type;
}

bool Tile::isSolid() const
{
	return solid;
}

SDL_Rect Tile::getTileSpace() const
{
	return tileSpace;
}
