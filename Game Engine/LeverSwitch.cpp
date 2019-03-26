#include "LeverSwitch.h"



LeverSwitch::LeverSwitch(int x, int y, int size) :
	Switch(x, y, size, 32, false) //will likely be isSolid = true later, temp false for testing
{
}

LeverSwitch::LeverSwitch(const LeverSwitch & t)
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
	renderAngle = t.getRenderAngle();
	pressedState = t.getPressedState();
	connectedToggleables = t.getConnectedToggleables();
	switchHitBox = t.getSwitchHitBox();
}


LeverSwitch::~LeverSwitch()
{
}

Tile * LeverSwitch::clone() const
{
	return new LeverSwitch(*this);
}

void LeverSwitch::setPressedState(bool pressed)
{
	if (pressedState != pressed)
	{
		toggleToggleables();
	}
	pressedState = pressed;
}

void LeverSwitch::entityEnteredTile(Entity * e)
{
	if (SDL_HasIntersection(e->getGroundHitBox(), switchHitBox))
	{
		if (!isEntityCurrentlyOn)
		{
			isEntityCurrentlyOn = true;
			setPressedState(!pressedState);
		}
	}
	else
	{
		isEntityCurrentlyOn = false;
	}
}

void LeverSwitch::incrementAnimationFrame()
{
	if (pressedState && animationFrame < 4)
	{
		animationFrame++;
	}
	else if (!pressedState && animationFrame > 0)
	{
		animationFrame--;
	}
}
