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
	int selectedTileType = 0;
};

