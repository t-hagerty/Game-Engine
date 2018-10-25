#include "Tile.h"



Tile::Tile(int x, int y, int size, int typeOfTile, bool isSolid, bool isAPit, TileEffect* effect)
{
	tileSpace = new SDL_Rect();
	tileSpace->x = x;
	tileSpace->y = y;
	tileSpace->w = size;
	tileSpace->h = size;
	type = typeOfTile;
	solid = isSolid;
	pit = isAPit;
	anEffect = effect;
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

bool Tile::isAPit() const
{
	return pit;
}

SDL_Rect* Tile::getTileSpace() const
{
	return tileSpace;
}

float Tile::getCenterPosX()
{
	return tileSpace->x + (tileSpace->w / 2);
}

float Tile::getCenterPosY()
{
	return tileSpace->y + (tileSpace->h / 2);
}

TileEffect * Tile::getTileEffect()
{
	return anEffect;
}

void Tile::setTileEffect(TileEffect * newEffect)
{
	anEffect = newEffect;
}
