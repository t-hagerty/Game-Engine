#include "Switch.h"



Switch::Switch(int x, int y, int size) :
	Tile(x, y, size, 30, false, false, nullptr)
{

}

Switch::~Switch()
{
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

std::vector<Toggleable*> Switch::getConnectedToggleables()
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
}

bool Switch::getPressedState()
{
	return pressedState;
}

void Switch::entityEnteredTile(Entity * e)
{
	setPressedState(true);
	//how to know when no entity on the switch anymore for the weighted switch?
}
