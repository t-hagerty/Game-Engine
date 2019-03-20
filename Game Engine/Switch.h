#pragma once
#include "Tile.h"
#include "Toggleable.h"
#include <vector>
class Switch :
	public Tile
{
public:
	Switch(int x = 0, int y = 0, int size = 0);
	Switch(const Switch &t);
	~Switch();
	Tile* clone() const override;
	void addToggleable(Toggleable* t);
	void removeToggleable(Toggleable* t);
	void clearToggleables();
	std::vector<Toggleable*> getConnectedToggleables() const;
	void setPressedState(bool pressed);
	bool getPressedState() const;
	virtual void entityEnteredTile(Entity * e) override;
	void incrementAnimationFrame() override;
protected:
	void toggleToggleables();
	std::vector<Toggleable*> connectedToggleables;
	bool pressedState = false;
};

