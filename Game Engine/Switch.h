#pragma once
#include "Tile.h"
#include "Toggleable.h"
#include <vector>
class Switch :
	public Tile
{
public:
	Switch(int x = 0, int y = 0, int size = 0);
	~Switch();
	void addToggleable(Toggleable* t);
	void removeToggleable(Toggleable* t);
	void clearToggleables();
	std::vector<Toggleable*> getConnectedToggleables();
	void setPressedState(bool pressed);
	bool getPressedState();
	virtual void entityEnteredTile(Entity * e) override;
protected:
	void toggleToggleables();
	std::vector<Toggleable*> connectedToggleables;
	bool pressedState = false;
};

