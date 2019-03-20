#include "Switch.h"



Switch::Switch(int x, int y, int size) :
	Tile(x, y, size, 30, false, false, nullptr)
{

}

Switch::Switch(const Switch & t)
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
}

Switch::~Switch()
{
}

Tile * Switch::clone() const
{
	return new Switch(*this);
}

void Switch::addToggleable(Toggleable * t)
{
	connectedToggleables.insert(connectedToggleables.end(), t);
}

void Switch::removeToggleable(Toggleable * t)
{
	for (int i = 0; i < connectedToggleables.size(); i++)
	{
		if (t == connectedToggleables.at(i))
		{
			connectedToggleables.erase(connectedToggleables.begin() + i);
			return;
		}
	}
}

void Switch::clearToggleables()
{
	connectedToggleables.clear();
}

std::vector<Toggleable*> Switch::getConnectedToggleables() const
{
	return connectedToggleables;
}

void Switch::toggleToggleables()
{
	for (Toggleable* t : connectedToggleables)
	{
		t->toggle();
	}
}

void Switch::setPressedState(bool pressed)
{
	if (pressedState != pressed)
	{
		toggleToggleables();
	}
	pressedState = pressed;
	if (pressedState)
	{
		animationFrame = 1;
	}
	else
	{
		animationFrame = 0;
	}
}

bool Switch::getPressedState() const
{
	return pressedState;
}

void Switch::entityEnteredTile(Entity * e)
{
	setPressedState(true);
	//how to know when no entity on the switch anymore for the weighted switch?
}

void Switch::incrementAnimationFrame()
{
}
