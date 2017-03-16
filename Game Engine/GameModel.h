#pragma once
#include <vector>
#include "Entity.h"

class GameModel
{
public:
	GameModel();
	~GameModel();
	void setAreaWidth(int width);
	void setAreaHeight(int height);
	void addEntity(Entity* newEntity);
	void moveEntities(double delta);
	Entity* getEntity(int index);
	int getNumberOfEntities() const;
	Entity* getPlayer() const;
private:
	int gameAreaWidth = 0; //Should be the same as the screen width, passed on through the controller
	int gameAreaHeight = 0; //Should be the same as the screen height, passed on through the controller
	std::vector<Entity*> entities;
	Entity* player;
	void moveAnEntity(Entity* e, double delta) const;
};

