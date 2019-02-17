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
	bool openMap() override;
private:
	void replaceTile(int row, int col, Tile* newTile);
	void removeExit();
	void placeWall(int row, int col);
	void selectWallType(int row, int col);
	int selectedTileType = 0;
	int wallTypeSelector[16] = { WALL_SINGLE, WALL_BOTTOM_END, WALL_LEFT_END, WALL_BOTTOM_LEFT_CORNER, WALL_TOP_END, WALL_VERTICAL, WALL_TOP_LEFT_CORNER, WALL_VERTICAL_LEFT, WALL_RIGHT_END, WALL_BOTTOM_RIGHT_CORNER, WALL_HORIZONTAL, WALL_HORIZONTAL_BOTTOM, WALL_TOP_RIGHT_CORNER, WALL_VERTICAL_RIGHT, WALL_HORIZONTAL_TOP, WALL_FILLED};
};

