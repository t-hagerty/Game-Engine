#pragma once
#include "Model.h"

class GameModel :
	public Model
{
public:
	GameModel();
	GameModel(std::vector<Tile*> map,int rows, int cols, std::vector<Entity*> levelEntities);
	~GameModel();
	void moveEntities(double delta);
	bool getIsGameOver();
	bool getIsLevelWon();
	bool openMap() override;
	void addEntityFromFile(Entity* e) override;
	void resetLevel();
private:
	int screenWidth;
	int screenHeight;
	std::vector<Entity*> entitiesInitialState; //For a game over, we have a copy of how every entity starts rather than have to reload the whole level
	bool isGameOver = false;
	bool isLevelWon = false;
	TileEffect* setTileEffect(int tileType) override;
	void moveAnEntity(Entity* e, double delta);
	void knockback(Entity* knockerbacker, Entity* knockedback);
	bool isCompletelyOverPit(Entity* entity, int topRow, int bottomRow, int leftCol, int rightCol);
	bool isCompletelyOverExit(Entity* entity, int topRow, int bottomRow, int leftCol, int rightCol);
	void fallIntoPit(Entity* e);
	void killEntity(Entity* e);
	void winLevel();
	TileEffect* downTreadmillEffect = new TileEffect(1, 1, 0, 0.4f);
	TileEffect* rightTreadmillEffect = new TileEffect(1, 1, 0.4f, 0);
	TileEffect* leftTreadmillEffect = new TileEffect(1, 1, -0.4f, 0);
	TileEffect* upTreadmillEffect = new TileEffect(1, 1, 0, -0.4f);
	TileEffect* iceEffect = new TileEffect(0.9f , 1.9f, 0, 0);
	TileEffect* mudEffect = new TileEffect(0.4f, 1, 0, 0, -1);
	TileEffect* lavaEffect = new TileEffect(0.7f, 1, 0, 0, -0.5f, 0, 1);
	TileEffect* spikeEffect = new TileEffect(1, 1, 0, 0, 0, 4, 2);
};

