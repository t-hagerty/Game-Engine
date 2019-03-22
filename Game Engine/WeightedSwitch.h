#pragma once
#include "Switch.h"
class WeightedSwitch :
	public Switch
{
public:
	WeightedSwitch(int x = 0, int y = 0, int size = 0);
	WeightedSwitch(const WeightedSwitch &t);
	~WeightedSwitch();
	Tile* clone() const override;
	void entityEnteredTile(Entity * e) override;
	void incrementAnimationFrame() override;
};

