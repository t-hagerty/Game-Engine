#pragma once
#include "Model.h"

class EditorModel :
	public Model
{
public:
	EditorModel();
	~EditorModel();
	void setSelectedTileType(int newType);
	int getSelectedTileType();
	void clickTile(int x, int y);
	void clickConfigureTile(int x, int y);
	bool getIsConfigureMode();
	bool openMap() override;
private:
	void replaceTile(int row, int col, Tile* newTile);
	void removeExit();
	int determineDoorDirection(int row, int col);
	void placeWall(int row, int col);
	void updateSurroundingWalls(int row, int col);
	void selectWallType(int row, int col);
	bool isAWall(int type);
	int selectedTileType = 0;
	bool isConfigurableTable[NUMBER_TILE_TYPES] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, true, true, false, true, true, true };
	Tile* configuredTile;
};

