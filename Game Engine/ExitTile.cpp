#include "ExitTile.h"



ExitTile::ExitTile(int x, int y, int size, int typeOfTile, bool isSolid, bool isAPit, TileEffect* effect, bool locked, short exitDir) :
	Tile(x, y, size, typeOfTile, false, false, effect)
{
	exitDirection = exitDir;
	renderAngle = exitDirection * 90;
	isLocked = locked;
	if (isLocked)
	{
		animationFrame = 0;
	}
}

ExitTile::ExitTile(const ExitTile & t)
{
	tileSpace = new SDL_Rect();
	tileSpace->x = t.getTileSpace()->x;
	tileSpace->y = t.getTileSpace()->y;
	tileSpace->w = t.getTileSpace()->w;
	tileSpace->h = t.getTileSpace()->h;
	type = t.getType();
	isLocked = t.getIsLocked();
	solid = isLocked;
	pit = t.isAPit();
	anEffect = nullptr;
	textureFrames = setNumberFrames();
	exitDirection = t.getExitDirection();
	renderAngle = t.getRenderAngle();
	if (isLocked)
	{
		animationFrame = 0;
	}
}


ExitTile::~ExitTile()
{
}

Tile * ExitTile::clone() const
{
	return new ExitTile(*this);
}

bool ExitTile::isSolid()
{
	if (isLocked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ExitTile::setIsLocked(bool locked)
{
	isLocked = locked;
}

bool ExitTile::getIsLocked() const
{
	return isLocked;
}

short ExitTile::getExitDirection() const
{
	return exitDirection;
}

void ExitTile::toggle()
{
	isLocked = !isLocked;
}

void ExitTile::incrementAnimationFrame()
{
	if (isLocked && animationFrame >= 1)
	{
		animationFrame--;
		//if it's not at first frame, go backwards until it's "closed"
		return;
	}
	if (!isLocked && animationFrame < textureFrames - 1)
	{
		animationFrame++;
		//if not locked and isnt at last frame, increment until last
		return;
	}
}
