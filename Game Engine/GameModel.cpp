#include "GameModel.h"
#include "Rectangle.h"
#include <SDL.h>
#include <iostream>
#include "Player.h"


GameModel::GameModel()
{
	tileSize = 32; //TODO change later so that this size adjusts based on the size of the screen/window
	openMap("testMap");
	player = new Player(64, 64, 60, 80, 0, 0, 10);
	addEntity(player);
	levelHeight = mapRows * tileSize;
	levelWidth = mapCols * tileSize;
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
	//moveAnEntity(player, delta);
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

Player * GameModel::getPlayer() const
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

int GameModel::getLevelWidth() const
{
	return levelWidth;
}

int GameModel::getLevelHeight() const
{
	return levelHeight;
}

std::vector<Tile*> GameModel::getTileMap() const
{
	return tileMap;
}

Tile * GameModel::getTileAtMapIndex(int row, int col) const
{
	return tileMap.at((row*mapCols) + col);
}

bool GameModel::setIsSolid(int tileType)
{
	return isSolidTable[tileType];
}

void GameModel::moveAnEntity(Entity * e, double delta) const
{
	double oldX = e->getPosX();
	double oldY = e->getPosY();
	int posRowTop = oldY / tileSize;
	int posRowBottom = (oldY + e->getHeight()) / tileSize;
	int posColLeft = oldX / tileSize;
	int posColRight = (oldX + e->getWidth()) / tileSize;

	e->determineMovement(player->getPosX(), player->getPosY());
	//=== Y MOVEMENT (UP(neg)/DOWN(pos)) ===
	e->setPosY(e->getPosY() + (e->getVelocityY() * delta));

	if (e->getPosY() <= 0) //If movement puts past the top bound of screen, put it against the top bound instead and set velocity to zero
	{
		e->setPosY(0);
		e->setVelocityY(0);
	}
	else if ((e->getPosY() + e->getHeight()) >= levelHeight) //If movement puts past the bottom bound of screen, put it against the bottom bound instead and set velocity to zero
	{
		e->setPosY(levelHeight - e->getHeight());
		e->setVelocityY(0);
	}
	if (e->getVelocityY() > 0) //down
	{
		/*if (e->getHeight() == 50 && e->getWidth() == 50)
		{
			std::cout << oldY;
			std::cout << ": ";
			std::cout << posRowTop;
			std::cout << ", ";
			std::cout << oldY + e->getHeight();
			std::cout << ", ";
			std::cout << posRowBottom;
			std::cout << "\n";
			std::cout << ((oldY + e->getHeight()) / tileSize);
			std::cout << "\n";
			int temp = (oldY + e->getHeight()) / tileSize;
			std::cout << temp;
			std::cout << "\n";
		}*/
		if ((posRowBottom) < mapRows && (isInsideWall(e, getTileAtMapIndex(posRowBottom, posColLeft))
			|| (posColRight < mapCols && isInsideWall(e, getTileAtMapIndex(posRowBottom, posColRight)))))
		{
			e->setPosY((posRowBottom) * tileSize - e->getHeight());
		}
		for (Entity* anotherEntity : entities)
		{
			if (e != anotherEntity && isIntersectingEntity(e, anotherEntity))
			{
				e->setPosY(anotherEntity->getPosY() - e->getHeight());
				//TODO Execute collision behavior (eg: damage, knockback)
			}
		}
	}
	else if (e->getVelocityY() < 0) //up
	{
		if ((posRowTop - 1) >= 0 && (isInsideWall(e, getTileAtMapIndex(posRowTop - 1, posColLeft))
			|| (posColRight < mapCols && isInsideWall(e, getTileAtMapIndex(posRowTop - 1, posColRight)))))
		{
			e->setPosY((posRowTop)* tileSize);
		}
		for (Entity* anotherEntity : entities)
		{
			if (e != anotherEntity && isIntersectingEntity(e, anotherEntity))
			{
				e->setPosY(anotherEntity->getPosY() + anotherEntity->getHeight());
				//TODO Execute collision behavior (eg: damage, knockback)
			}
		}
	}

	//=== X MOVEMENT (LEFT(neg)/RIGHT(pos)) ===
	e->setPosX(e->getPosX() + (e->getVelocityX() * delta));

	if (e->getPosX() <= 0) //If movement puts past the left bound of screen, put it against the left bound instead and set velocity to zero
	{
		e->setPosX(0);
		e->setVelocityX(0);
	}
	else if ((e->getPosX() + e->getWidth()) >= levelWidth) //If movement puts past the right bound of screen, put it against the right bound instead and set velocity to zero
	{
		e->setPosX(levelWidth - e->getWidth());
		e->setVelocityX(0);
	}

	if (e->getVelocityX() > 0) //right
	{
		if ((posColRight) < mapCols && (isInsideWall(e, getTileAtMapIndex(posRowTop, posColRight))
			|| (posRowBottom < mapRows && isInsideWall(e, getTileAtMapIndex(posRowBottom, posColRight))))) //TODO
		{
			e->setPosX((posColRight) * tileSize - e->getWidth());
		}
		for(Entity* anotherEntity : entities)
		{
			if(e != anotherEntity && isIntersectingEntity(e, anotherEntity))
			{
				e->setPosX(anotherEntity->getPosX() - e->getWidth());
				//TODO Execute collision behavior (eg: damage, knockback)
			}
		}
	}
	else if (e->getVelocityX() < 0) //left
	{
		if ((posColLeft - 1) >= 0 && (isInsideWall(e, getTileAtMapIndex(posRowTop, posColLeft - 1))
			|| (posRowBottom < mapRows && isInsideWall(e, getTileAtMapIndex(posRowBottom, posColLeft - 1))))
				|| (e->getVelocityY() < 0 && (posRowTop - 1 >= 0 && isInsideWall(e, getTileAtMapIndex(posRowTop - 1, posColLeft - 1))))) //left
		{
			e->setPosX((posColLeft)* tileSize);
		}
		for (Entity* anotherEntity : entities)
		{
			if (e != anotherEntity && isIntersectingEntity(e, anotherEntity))
			{
				e->setPosX(anotherEntity->getPosX() + anotherEntity->getWidth());
				//TODO Execute collision behavior (eg: damage, knockback)
			}
		}
	}

	//Safety check, make sure once we're done, the entity didnt manage to still make it inside a wall, if so, fallback on moving it back to former pos:
	if(isInsideWall(e, getTileAtMapIndex(e->getPosY() / tileSize, e->getPosX() / tileSize)))
	{
		e->setPosX(oldX);
		e->setPosY(oldY);
	}
	for (Entity* anotherEntity : entities)
	{
		if (e != anotherEntity && isIntersectingEntity(e, anotherEntity))
		{
			e->setPosX(oldX);
			e->setPosY(oldY);
			//TODO Execute collision behavior (eg: damage, knockback)
		}
	}
}

bool GameModel::isInsideWall(Entity* entity, Tile * t)
{
	if(!t->isSolid())
	{
		return false;
	}
	if(SDL_HasIntersection(&t->getTileSpace(), entity->getCollisionBox()))
	{
		return true;
	}
	return false;
}

bool GameModel::isInsideAnyWalls(Entity * entity, int topRow, int bottomRow, int leftCol, int rightCol) const
{
	for(int r = topRow; r <= bottomRow; r++)
	{
		for(int c = leftCol; c <= rightCol; c++)
		{	
			if(isInsideWall(entity, getTileAtMapIndex(r, c)))
			{
				return true;
			}
		}
	}
	return false;
}

bool GameModel::isIntersectingEntity(Entity* e1, Entity* e2)
{
	if (SDL_HasIntersection(e1->getCollisionBox(), e2->getCollisionBox()))
	{
		return true;
	}
	return false;
}

bool GameModel::openMap()
{
	bool success = true;
	mapRows = 20;
    mapCols = 30;
	int testMap[20][30] = { { 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 2 } };
	tileMap.reserve(mapRows * mapCols);
	for (int r = 0; r < mapRows; r++)
	{
		for(int c = 0; c < mapCols; c++)
		{
			tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, testMap[r][c], setIsSolid(testMap[r][c])));
		}
	}
	return success;
}

bool GameModel::openMap(std::string filePath)
{
	bool success = true;

	SDL_RWops* file = SDL_RWFromFile(filePath.c_str(), "r+b");
	if(file == nullptr)
	{
		printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());
		success = false;
		openMap(); //Will be handled better in the future, for now, just load the default map
	}
	else
	{
		//Load data
		printf("Reading file...!\n");
		SDL_RWread(file, &mapRows, sizeof(Sint32), 1); //read number of rows of saved map
		SDL_RWread(file, &mapCols, sizeof(Sint32), 1); //read number of cols of saved map

		tileMap.reserve(mapRows * mapCols);
		for (int r = 0; r < mapRows; r++)
		{
			for (int c = 0; c < mapCols; c++)
			{
				Sint32 tempType = -1;
				SDL_RWread(file, &tempType, sizeof(Sint32), 1);
				tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, tempType, setIsSolid(tempType)));
			}
		}

		//Close file handler
		SDL_RWclose(file);
	}

	return success;
}

bool GameModel::saveMap(std::string filePath) const
{
	bool success = true;

	SDL_RWops* file = SDL_RWFromFile(filePath.c_str(), "w+b");
	SDL_RWwrite(file, &mapRows, sizeof(Sint32), 1);
	SDL_RWwrite(file, &mapCols, sizeof(Sint32), 1);
	if (file != nullptr)
	{
		//Save data
		for (int r = 0; r < mapRows; r++)
		{
			for(int c = 0; c < mapCols; c++)
			{
				//print data for debugging:
				//std::cout << tileMap[r][c];
				//std::cout << ", ";
				int tempType = tileMap[(r*mapCols) + c]->getType();
				SDL_RWwrite(file, &tempType, sizeof(Sint32), 1);
			}
			//std::cout << "\n";
		}

		//Close file handler
		SDL_RWclose(file);
	}
	else
	{
		printf("Error: Unable to save file! %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

void GameModel::deleteMap() const
{
	for (int i = 0; i < mapRows*mapCols; ++i) 
	{
		delete &tileMap[i];
	}
	delete &tileMap;
}
