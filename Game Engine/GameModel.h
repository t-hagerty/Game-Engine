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
	bool openMap() override;
	void addEntityFromFile(Entity* e) override;
	void resetLevel();
private:
	int screenWidth;
	int screenHeight;
	std::vector<Entity*> entitiesInitialState; //For a game over, we have a copy of how every entity starts rather than have to reload the whole level
	bool isGameOver = false;
	TileEffect* setTileEffect(int tileType) override;
	void moveAnEntity(Entity* e, double delta);
	void calculateMovement(Entity* e, int posRowTop, int posRowBottom, int posColLeft, int posColRight);
	//void checkEntityMovementX(Entity* e, double delta);
	//void checkEntityMovementY(Entity* e, double delta);
	void knockback(Entity* knockerbacker, Entity* knockedback);
	bool isCompletelyOverPit(Entity* entity, int topRow, int bottomRow, int leftCol, int rightCol);
	void fallIntoPit(Entity* e);
	void killEntity(Entity* e);
	TileEffect* downTreadmillEffect = new TileEffect(1, 1, 0, 0.4);
	TileEffect* rightTreadmillEffect = new TileEffect(1, 1, 0.4, 0);
	TileEffect* leftTreadmillEffect = new TileEffect(1, 1, -0.4, 0);
	TileEffect* upTreadmillEffect = new TileEffect(1, 1, 0, -0.4);
	TileEffect* iceEffect = new TileEffect(0.9 , 1.9, 0, 0);
	TileEffect* mudEffect = new TileEffect(0.4, 1, 0, 0, -1);
	TileEffect* lavaEffect = new TileEffect(0.7, 1, 0, 0, -0.5, 0, 1);
	TileEffect* spikeEffect = new TileEffect(1, 1, 0, 0, 0, 4, 2);
};

