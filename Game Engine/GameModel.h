#pragma once
#include <vector>
#include "Entity.h"
#include "SDL_endian.h"
#include "Tile.h"
#include "TileEffect.h"
#include "Player.h"
#include "Enemy.h"
#include "Arrow.h"

class GameModel
{
public:
	GameModel();
	~GameModel();
	void addEntity(Entity* newEntity);
	void moveEntities(double delta);
	Entity* getEntity(int index);
	void removeEntity(Entity* e);
	int getNumberOfEntities() const;
	Player* getPlayer() const;
	bool getIsGameOver();
	int getTileSize() const;
	int getMapRows() const;
	int getMapCols() const;
	int getLevelWidth() const;
	int getLevelHeight() const;
	std::vector<Tile*> getTileMap() const;
	bool openMap();
	bool openMap(std::string filePath);
	bool saveMap(std::string filePath) const;
	void resetLevel();
private:
	int screenWidth;
	int screenHeight;
	int tileSize;
	const static int NUMBER_TILE_TYPES = 18;
	int levelWidth;
	int levelHeight;
	std::vector<Tile*> tileMap;
	Tile* getTileAtMapIndex(int row, int col) const;
	int mapRows;
	int mapCols;
	std::vector<Entity*> entities;
	std::vector<Entity*> entitiesInitialState; //For a game over, we have a copy of how every entity starts rather than have to reload the whole level
	Player* player;
	bool isGameOver = false;
	bool setIsSolid(int tileType);
	TileEffect* setTileEffect(int tileType);
	void moveAnEntity(Entity* e, double delta);
	void knockback(Entity* knockerbacker, Entity* knockedback);
	static bool isInsideWall(Entity* entity, Tile* t);
	bool isInsideAnyWalls(Entity* entity, int topRow, int bottomRow, int leftCol, int rightCol) const;
	static bool isIntersectingEntity(Entity* e1, Entity* e2);
	void slideIntoPit(Entity* entity, int topRow, int bottomRow, int leftCol, int rightCol);
	bool isCompletelyOverPit(Entity* entity, int topRow, int bottomRow, int leftCol, int rightCol);
	void fallIntoPit(Entity* e);
	void killEntity(Entity* e);
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
		BARRIER,
		DOWN_TREADMILL,
		RIGHT_TREADMILL, //10
		LEFT_TREADMILL,
		UP_TREADMILL,
		ICE,
		MUD,
		PIT,
		LAVA,
		SPIKES
	};
	bool isSolidTable[NUMBER_TILE_TYPES] = { false, true, true, true, true, true, true, false, true, false, false, false, false, false, false, false, false, false };
	TileEffect* downTreadmillEffect = new TileEffect(1, 1, 0, 0.4);
	TileEffect* rightTreadmillEffect = new TileEffect(1, 1, 0.4, 0);
	TileEffect* leftTreadmillEffect = new TileEffect(1, 1, -0.4, 0);
	TileEffect* upTreadmillEffect = new TileEffect(1, 1, 0, -0.4);
	TileEffect* iceEffect = new TileEffect(0.9 , 1.9, 0, 0);
	TileEffect* mudEffect = new TileEffect(0.4, 1, 0, 0, -1);
	TileEffect* lavaEffect = new TileEffect(0.7, 1, 0, 0, -0.5, 0, 1);
	TileEffect* spikeEffect = new TileEffect(1, 1, 0, 0, 0, 4, 2);
};

