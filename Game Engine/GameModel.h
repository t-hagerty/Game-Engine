#pragma once
#include <vector>
#include "Entity.h"

class GameModel
{
public:
	GameModel(int width, int height);
	~GameModel();
	void addEntity(Entity* newEntity);
	void moveEntities(double delta);
	Entity* getEntity(int index);
	int getNumberOfEntities() const;
	Entity* getPlayer() const;
	int getTileSize() const;
	int getMapRows() const;
	int getMapCols() const;
	int** getTileMap() const;
private:
	int screenWidth;
	int screenHeight;
	int tileSize;
	int numTileTypes;
	int levelWidth;
	int levelHeight;
	int **tileMap;
	int mapRows;
	int mapCols;
	std::vector<Entity*> entities;
	Entity* player;
	void moveAnEntity(Entity* e, double delta) const;
	void createMap();
	void createMap(std::string filePath);
	void deleteMap() const;
	enum textures
	{
		FLOOR,
		WALL_BOTTOM_LEFT_CORNER,
		WALL_BOTTOM_RIGHT_CORNER,
		WALL_TOP_LEFT_CORNER,
		WALL_TOP_RIGHT_CORNER,
		WALL_HORIZONTAL,
		WALL_VERTICAL,
		GRASS,
		BARRIER
	};
};

