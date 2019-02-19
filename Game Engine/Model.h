#pragma once
#include <vector>
#include "Entity.h"
#include "SDL_endian.h"
#include "Tile.h"
#include "TileEffect.h"
#include "ExitTile.h"
#include "Player.h"
#include "Enemy.h"
#include "Arrow.h"
class Model
{
public:
	Model();
	~Model();
	void addEntity(Entity* newEntity);
	Entity* getEntity(int index);
	std::vector<Entity*> getEntities();
	void removeEntity(Entity* e);
	int getNumberOfEntities() const;
	Player* getPlayer() const;
	ExitTile* getExit() const;
	int getTileSize() const;
	int getMapRows() const;
	int getMapCols() const;
	int getLevelWidth() const;
	int getLevelHeight() const;
	std::vector<Tile*> getTileMap() const;
	virtual bool openMap() = 0;
	virtual bool openMap(std::string filePath);
	virtual void addEntityFromFile(Entity* e);
	bool saveMap(std::string filePath) const;
protected:
	int tileSize;
	const static int NUMBER_TILE_TYPES = 30;
	int levelWidth;
	int levelHeight;
	std::vector<Tile*> tileMap;
	bool setIsSolid(int tileType);
	virtual TileEffect* setTileEffect(int tileType);
	Tile* getTileAtMapIndex(int row, int col) const;
	int mapRows;
	int mapCols;
	std::vector<Entity*> entities;
	Player* player;
	ExitTile* exit;
	static bool isInsideWall(Entity* entity, Tile* t);
	bool isInsideAnyWalls(Entity* entity, int topRow, int bottomRow, int leftCol, int rightCol) const;
	static bool isIntersectingEntity(Entity* e1, Entity* e2);
	void deleteMap() const;
	enum textures
	{
		WALL_SINGLE,
		WALL_BOTTOM_END,
		WALL_LEFT_END,
		WALL_BOTTOM_LEFT_CORNER,
		WALL_TOP_END,
		WALL_VERTICAL,
		WALL_TOP_LEFT_CORNER,
		WALL_VERTICAL_LEFT,
		WALL_RIGHT_END,
		WALL_BOTTOM_RIGHT_CORNER,
		WALL_HORIZONTAL,
		WALL_HORIZONTAL_BOTTOM,
		WALL_TOP_RIGHT_CORNER,
		WALL_VERTICAL_RIGHT,
		WALL_HORIZONTAL_TOP,
		WALL_FILLED,
		FLOOR,
		GRASS,
		BARRIER,
		DOWN_TREADMILL,
		RIGHT_TREADMILL,
		LEFT_TREADMILL,
		UP_TREADMILL,
		ICE,
		MUD,
		PIT,
		LAVA,
		SPIKES,
		DOOR,
		LADDER,

		PLAYER,
		ENEMY,
		ARROW
	};
	bool isSolidTable[NUMBER_TILE_TYPES] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, true, false, false, false, false, false, false, false, false, false, false, false };
};

