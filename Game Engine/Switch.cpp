#include "Switch.h"



Switch::Switch(int x, int y, int size) :
	Tile(x, y, size, 30, false, false, nullptr)
{
	switchHitBox = new SDL_Rect();
	int hitboxOffset = (((size / 8) * 3) / 2);
	switchHitBox->x = x + hitboxOffset;
	switchHitBox->y = y + hitboxOffset;
	switchHitBox->w = (size / 8) * 5;
	switchHitBox->h = switchHitBox->w;
}

Switch::Switch(int x, int y, int size, int typeOfTile, bool isSolid) :
	Tile(x, y, size, typeOfTile, isSolid, false, nullptr)
{
	switchHitBox = new SDL_Rect();
	int hitboxOffset = (((size / 8) * 3) / 2);
	switchHitBox->x = x + hitboxOffset;
	switchHitBox->y = y + hitboxOffset;
	switchHitBox->w = (size / 8) * 5;
	switchHitBox->h = switchHitBox->w;
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
	switchHitBox = t.getSwitchHitBox();
}

Switch::~Switch()
{
	delete switchHitBox;
}

Tile * Switch::clone() const
{
	return new Switch(*this);
}

bool Switch::addToggleable(Toggleable * t)
{
	for (Toggleable* ct : connectedToggleables)
	{
		if (ct == t)
		{
			return false;
		}
	}
	connectedToggleables.insert(connectedToggleables.end(), t);
	return true;
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

SDL_Rect * Switch::getSwitchHitBox() const
{
	return switchHitBox;
}

void Switch::setIsHighlighted(bool highlighted)
{
	isHighlighted = highlighted;
	for (Toggleable* t : connectedToggleables)
	{
		Tile *aTile = dynamic_cast<Tile*>(t);
		if (aTile != nullptr)
		{
			aTile->setIsHighlighted(highlighted);
		}
	}
}

void Switch::entityEnteredTile(Entity * e)
{
	if (SDL_HasIntersection(e->getGroundHitBox(), switchHitBox))
	{
		setPressedState(true);
	}
}

void Switch::incrementAnimationFrame()
{
}
