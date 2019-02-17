#include "ExitTile.h"



ExitTile::ExitTile(int x, int y, int size, int typeOfTile, bool isSolid, bool isAPit, TileEffect* effect, bool locked, short exitDir) :
	Tile(x, y, size, typeOfTile, false, false, effect)
{
	exitDirection = exitDir;
	isLocked = locked;
	if (isLocked)
	{
		animationFrame = 1;
	}
}


ExitTile::~ExitTile()
{
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

bool ExitTile::getIsLocked()
{
	return isLocked;
}

short ExitTile::getExitDirection()
{
	return exitDirection;
}

void ExitTile::switchTriggered(bool switchStatus)
{
	isLocked = !switchStatus;
}

void ExitTile::incrementAnimationFrame()
{
	if (isLocked && animationFrame > 1)
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
