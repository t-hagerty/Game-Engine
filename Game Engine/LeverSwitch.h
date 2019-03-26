#pragma once
#include "Switch.h"
class LeverSwitch :
	public Switch
{
public:
	LeverSwitch(int x = 0, int y = 0, int size = 0);
	LeverSwitch(const LeverSwitch &t);
	~LeverSwitch();
	Tile* clone() const override;
	void setPressedState(bool pressed) override;
	void entityEnteredTile(Entity * e) override;
	void incrementAnimationFrame() override;
private:
	bool isEntityCurrentlyOn = false;
};

