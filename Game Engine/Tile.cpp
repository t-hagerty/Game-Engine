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

Tile::Tile(const Tile & t)
{
	tileSpace = new SDL_Rect();
	tileSpace->x = t.getTileSpace()->x;
	tileSpace->y = t.getTileSpace()->y;
	tileSpace->w = t.getTileSpace()->w;
	tileSpace->h = t.getTileSpace()->h;
	type = t.getType();
	solid = t.isSolid();
	pit = t.isAPit();
	anEffect = nullptr;
	textureFrames = setNumberFrames();
}


Tile::~Tile()
{
	delete tileSpace;
}

Tile * Tile::clone() const
{
	return new Tile(*this);
}

int Tile::getType() const
{
	return type;
}

bool Tile::isSolid() const
{
	return solid;
}

bool Tile::isSolid()
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
	case 19:
		return 8;
	case 20:
		return 8;
	case 21:
		return 8;
	case 22:
		return 8;
	case 26:
		return 8;
	case 28:
		return 8;
	default:
		return 1;
	}
}
