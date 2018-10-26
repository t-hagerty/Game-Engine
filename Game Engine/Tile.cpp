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
	textureFrames = setNumberFrames();
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

int Tile::getTextureFrameWidth() const
{
	return TEXTURE_FRAME_WIDTH;
}

int Tile::getTextureFrameHeight() const
{
	return TEXTURE_FRAME_HEIGHT;
}

int Tile::getTextureFrames() const
{
	return textureFrames;
}

int Tile::getAnimationFrame() const
{
	return animationFrame;
}

void Tile::setAnimationFrame(int newFrame)
{
	if (newFrame < textureFrames)
	{
		animationFrame = newFrame;
	}
	else
	{
		animationFrame = 0;
	}
}

void Tile::incrementAnimationFrame()
{
	if (animationFrame++ >= textureFrames - 1)
	{
		animationFrame = 0;
	}
}

int Tile::setNumberFrames() const
{
	switch (type)
	{
	case 9:
		return 8;
	case 10:
		return 8;
	case 11:
		return 8;
	case 12:
		return 8;
	case 16:
		return 8;
	default:
		return 1;
	}
}
