#pragma once
#include <vector>
#include "Entity.h"
#include "SDL_endian.h"
#include "Tile.h"

class GameModel
{
public:
	GameModel();
	~GameModel();
	void addEntity(Entity* newEntity);
	void moveEntities(double delta);
	Entity* getEntity(int index);
	int getNumberOfEntities() const;
	Entity* getPlayer() const;
	int getTileSize() const;
	int getMapRows() const;
	int getMapCols() const;
	int getLevelWidth() const;
	int getLevelHeight() const;
	std::vector<Tile*> getTileMap() const;
	bool openMap();
	bool openMap(std::string filePath);
	bool saveMap(std::string filePath) const;
private:
	int screenWidth;
	int screenHeight;
	int tileSize;
	const static int numTileTypes = 9;
	int levelWidth;
	int levelHeight;
	std::vector<Tile*> tileMap;
	Tile* convert2DCoordsToMapIndex(int row, int col) const;
	int mapRows;
	int mapCols;
	std::vector<Entity*> entities;
	Entity* player;
	bool setIsSolid(int tileType);
	void moveAnEntity(Entity* e, double delta) const;
	static bool isInsideWall(Entity* entity, Tile* t);
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
	bool isSolidTable[numTileTypes] = { false, true, true, true, true, true, true, false, true };
};

