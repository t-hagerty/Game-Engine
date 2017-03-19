#include "GameModel.h"
#include "Rectangle.h"


GameModel::GameModel(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	tileSize = 20;
	createMap();
	player = new Rectangle(10, 10, 20, 50, 0, 0);
}


GameModel::~GameModel()
{
}

void GameModel::addEntity(Entity * newEntity)
{
	entities.insert(entities.end(), newEntity);
}

void GameModel::moveEntities(double delta)
{
	for (auto* e : entities)
	{
		moveAnEntity(e, delta);
	}
	moveAnEntity(player, delta);
}

Entity * GameModel::getEntity(int index)
{
	if(index < static_cast<int>(entities.size()))
	{
		return entities.at(index);
	}
	else
	{
		return nullptr;
	}
}

int GameModel::getNumberOfEntities() const
{
	return entities.size();
}

Entity * GameModel::getPlayer() const
{
	return player;
}

int GameModel::getTileSize() const
{
	return tileSize;
}

int GameModel::getMapRows() const
{
	return mapRows;
}

int GameModel::getMapCols() const
{
	return mapCols;
}

int** GameModel::getTileMap() const
{
	return tileMap;
}

void GameModel::moveAnEntity(Entity * e, double delta) const
{
	e->setPosX(e->getPosX() + (e->getVelocityX() * delta));

	if (e->getPosX() <= 0) //If movement puts past the left bound of screen, put it against the left bound instead and set velocity to zero
	{
		e->setPosX(0);
		e->setVelocityX(0);
	}
	else if ((e->getPosX() + e->getWidth()) >= screenWidth) //If movement puts past the right bound of screen, put it against the right bound instead and set velocity to zero
	{
		e->setPosX(screenWidth - e->getWidth());
		e->setVelocityX(0);
	}

	e->setPosY(e->getPosY() + (e->getVelocityY() * delta));

	if (e->getPosY() <= 0) //If movement puts past the top bound of screen, put it against the top bound instead and set velocity to zero
	{
		e->setPosY(0);
		e->setVelocityY(0);
	}
	else if ((e->getPosY() + e->getHeight()) >= screenHeight) //If movement puts past the bottom bound of screen, put it against the bottom bound instead and set velocity to zero
	{
		e->setPosY(screenHeight - e->getHeight());
		e->setVelocityY(0);
	}
}

void GameModel::createMap()
{
	mapRows = 20;
    mapCols = 30;
	int testMap[20][30] = { { 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 2 } };
	tileMap = new int*[mapRows];
	for (int i = 0; i < mapRows; i++)
	{
		tileMap[i] = new int[mapCols];
		//tileMap[i] = testMap[i];
		for(int j = 0; j < mapCols; j++)
		{
			tileMap[i][j] = testMap[i][j];
		}
	}
}

void GameModel::createMap(std::string filePath)
{
	//TODO: Implement file reading
	//read number rows, columns from file

	tileMap = new int*[mapRows];
	for (int i = 0; i < mapRows; i++)
	{
		tileMap[i] = new int[mapCols];
		//for loop reading each number corresponding to a tile texture/type from left to right, top to bottom
	}
}

void GameModel::deleteMap() const
{
	for (int i = 0; i < mapRows; ++i) 
	{
		delete[] tileMap[i];
	}
	delete[] tileMap;
}
