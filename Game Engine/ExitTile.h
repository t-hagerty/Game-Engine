#pragma once
#include "Tile.h"
#include <functional>
class ExitTile :
	public Tile
{
public:
	ExitTile(int x = 0, int y = 0, int size = 0, int typeOfTile = -1, bool isSolid = false, bool isAPit = false, TileEffect* effect = nullptr, bool isLocked = false, short exitDir = 0);
	ExitTile(const ExitTile &t);
	~ExitTile();
	Tile* clone() const override;
	bool isSolid() override;
	void setIsLocked(bool locked);
	bool getIsLocked() const;
	short getExitDirection() const;
	void switchTriggered(bool switchStatus);
	void incrementAnimationFrame() override;
private:
	bool isLocked = false;
	short exitDirection = 0; //0 for up, 1 for right, 2 for down, 3 for left, 4 for ladder
	
};

