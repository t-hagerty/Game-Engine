#include "Model.h"



Model::Model()
{
}

Model::~Model()
{
	deleteMap();
	for (Entity* e : entities)
	{
		delete e;
	}
}

void Model::addEntity(Entity * newEntity)
{
	entities.insert(entities.end(), newEntity);
}

Entity * Model::getEntity(int index)
{
	if (index < static_cast<int>(entities.size()))
	{
		return entities.at(index);
	}
	else
	{
		return nullptr;
	}
}

std::vector<Entity*> Model::getEntities()
{
	return entities;
}

void Model::removeEntity(Entity * e)
{
	for (int i = 0; i < static_cast<int>(entities.size()); i++)
	{
		if (e == getEntity(i))
		{
			delete e;
			entities.erase(entities.begin() + i);
			return;
		}
	}
}

int Model::getNumberOfEntities() const
{
	return (int)entities.size();
}

Player * Model::getPlayer() const
{
	return player;
}

ExitTile * Model::getExit() const
{
	return exit;
}

int Model::getTileSize() const
{
	return tileSize;
}

int Model::getMapRows() const
{
	return mapRows;
}

int Model::getMapCols() const
{
	return mapCols;
}

int Model::getLevelWidth() const
{
	return levelWidth;
}

int Model::getLevelHeight() const
{
	return levelHeight;
}

std::vector<Tile*> Model::getTileMap() const
{
	return tileMap;
}

bool Model::openMap(std::string filePath)
{
	bool success = true;

	SDL_RWops* file = SDL_RWFromFile(filePath.c_str(), "r+b");
	if (file == nullptr)
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
				
				if (tempType == DOOR || tempType == LADDER)
				{
					bool tempLocked;
					Sint32 tempDirection = -1;
					SDL_RWread(file, &tempLocked, sizeof(bool), 1);
					SDL_RWread(file, &tempDirection, sizeof(Sint32), 1);
					exit = new ExitTile(c * tileSize, r * tileSize, tileSize, tempType, false, false, setTileEffect(tempType), tempLocked, tempDirection);
					tileMap.push_back(exit);
				}
				else
				{
					tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, tempType, setIsSolid(tempType), ((tempType == PIT) ? true : false), setTileEffect(tempType)));
				}
			}
		}
		int numEntities = 0;
		SDL_RWread(file, &numEntities, sizeof(Sint32), 1);
		for (int i = 1; i <= numEntities; i++)
		{
			char entityType;
			int height, width, posX, posY, velX, velY, hp;
			SDL_RWread(file, &entityType, sizeof(char), 1);
			Entity* anEntity;
			switch (entityType)
			{
			case 'P':
				SDL_RWread(file, &height, sizeof(Sint32), 1);
				SDL_RWread(file, &width, sizeof(Sint32), 1);
				SDL_RWread(file, &posX, sizeof(Sint32), 1);
				SDL_RWread(file, &posY, sizeof(Sint32), 1);
				SDL_RWread(file, &velX, sizeof(Sint32), 1);
				SDL_RWread(file, &velY, sizeof(Sint32), 1);
				SDL_RWread(file, &hp, sizeof(Sint32), 1);
				player = new Player(height, width, posX, posY, velX, velY, hp);
				addEntityFromFile(player);
				break;
			case 'A':
				SDL_RWread(file, &height, sizeof(Sint32), 1);
				SDL_RWread(file, &width, sizeof(Sint32), 1);
				SDL_RWread(file, &posX, sizeof(Sint32), 1);
				SDL_RWread(file, &posY, sizeof(Sint32), 1);
				SDL_RWread(file, &velX, sizeof(Sint32), 1);
				SDL_RWread(file, &velY, sizeof(Sint32), 1);
				anEntity = new Arrow(height, width, posX, posY, velX, velY);
				addEntityFromFile(anEntity);
				break;
			case 'E':
				SDL_RWread(file, &height, sizeof(Sint32), 1);
				SDL_RWread(file, &width, sizeof(Sint32), 1);
				SDL_RWread(file, &posX, sizeof(Sint32), 1);
				SDL_RWread(file, &posY, sizeof(Sint32), 1);
				SDL_RWread(file, &velX, sizeof(Sint32), 1);
				SDL_RWread(file, &velY, sizeof(Sint32), 1);
				SDL_RWread(file, &hp, sizeof(Sint32), 1);
				anEntity = new Enemy(height, width, posX, posY, velX, velY, hp);
				addEntityFromFile(anEntity);
				break;
			default:
				SDL_RWclose(file);
				printf("Error: Encountered unexpected file input of entity type %c from file %s \n", entityType, filePath);
				return false;
				break;
			}
		}
		//Close file handler
		SDL_RWclose(file);
	}

	return success;
}

void Model::addEntityFromFile(Entity * e)
{
	addEntity(e);
}

bool Model::saveMap(std::string filePath) const
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
			for (int c = 0; c < mapCols; c++)
			{
				int tempType = tileMap[(r*mapCols) + c]->getType();
				if (tempType == DOOR || tempType == LADDER)
				{
					bool tempLocked = dynamic_cast<ExitTile*>(tileMap[(r*mapCols) + c])->getIsLocked();
					Sint32 tempDirection = dynamic_cast<ExitTile*>(tileMap[(r*mapCols) + c])->getExitDirection();
					SDL_RWwrite(file, &tempType, sizeof(Sint32), 1);
					SDL_RWwrite(file, &tempLocked, sizeof(bool), 1);
					SDL_RWwrite(file, &tempDirection, sizeof(Sint32), 1);
				}
				else
				{
					SDL_RWwrite(file, &tempType, sizeof(Sint32), 1);
				}
			}
		}
		char temp = 'P'; //player
		int tempInt = getNumberOfEntities();
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1); //number of entities
		for (int i = 0; i < static_cast<int>(entities.size()); i++)
		{
			Entity* e = entities[i];
			char temp;
			Player *aPlayer = dynamic_cast<Player*>(e);
			Enemy *anEnemy = dynamic_cast<Enemy*>(e);
			Arrow *anArrow = dynamic_cast<Arrow*>(e);
			if (aPlayer != nullptr)
			{
				temp = 'P';
			}
			else if (anEnemy != nullptr)
			{
				temp = 'E';
			}
			else if (anArrow != nullptr)
			{
				temp = 'A';
			}
			SDL_RWwrite(file, &temp, sizeof(char), 1);
			tempInt = e->getHeight();
			SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
			tempInt = e->getWidth();
			SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
			tempInt = e->getPosX();
			SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
			tempInt = e->getPosY();
			SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
			tempInt = e->getVelocityX();
			SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
			tempInt = e->getVelocityY();
			SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
			if (temp != 'A')
			{
				tempInt = e->getHealth();
				SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
			}
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

bool Model::setIsSolid(int tileType)
{
	return isSolidTable[tileType];
}

TileEffect * Model::setTileEffect(int tileType)
{
	return nullptr;
}

Tile * Model::getTileAtMapIndex(int row, int col) const
{
	return tileMap.at((row*mapCols) + col);
}

bool Model::isInsideWall(Entity * entity, Tile * t)
{
	if (!t->isSolid())
	{
		return false;
	}
	if (SDL_HasIntersection(t->getTileSpace(), entity->getGroundHitBox()))
	{
		return true;
	}
	return false;
}

bool Model::isInsideAnyWalls(Entity * entity, int topRow, int bottomRow, int leftCol, int rightCol) const
{
	for (int r = topRow; r <= bottomRow; r++)
	{
		for (int c = leftCol; c <= rightCol; c++)
		{
			if (r < mapRows && r >= 0 && c >= 0 && c < mapCols && isInsideWall(entity, getTileAtMapIndex(r, c)))
			{
				return true;
			}
		}
	}
	return false;
}

bool Model::isIntersectingEntity(Entity * e1, Entity * e2)
{
	if (SDL_HasIntersection(e1->getCollisionBox(), e2->getCollisionBox()))
	{
		return true;
	}
	return false;
}

void Model::deleteMap() const
{
	for (int i = 0; i < mapRows*mapCols; ++i)
	{
		delete tileMap[i];
	}
}

void Model::printTileMap()
{
	for (int r = 0; r < mapRows; r++)
	{
		for (int c = 0; c < mapCols; c++)
		{
			printf("%2d, ", getTileAtMapIndex(r, c)->getType());
		}
		printf("\n");
	}
}
