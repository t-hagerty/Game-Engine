#include "WeightedSwitch.h"



WeightedSwitch::WeightedSwitch(int x, int y, int size) :
	Switch(x, y, size, 31, false)
{
}

WeightedSwitch::WeightedSwitch(const WeightedSwitch & t)
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


WeightedSwitch::~WeightedSwitch()
{
}

Tile * WeightedSwitch::clone() const
{
	return new WeightedSwitch(*this);
}

void WeightedSwitch::entityEnteredTile(Entity * e)
{
	if (SDL_HasIntersection(e->getGroundHitBox(), switchHitBox))
	{
		setPressedState(true);
	}
	else
	{
		setPressedState(false);
	}
}

void WeightedSwitch::incrementAnimationFrame()
{
}
