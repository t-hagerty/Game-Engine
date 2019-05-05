#include "Model.h"
#include <tuple>

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

template< typename T > std::vector<byte> toByteArray(const T& object, std::vector<byte> bytes)
{
	std::vector<byte> newBytes;

	const byte* begin = reinterpret_cast<const byte*>(std::addressof(object));
	const byte* end = begin + sizeof(T);
	std::copy(begin, end, std::begin(newBytes));
	bytes.insert(std::end(bytes), std::begin(newBytes), std::end(newBytes));
	return bytes;
}

bool Model::openMap(std::string filePath)
{
	SDL_RWops* mapData = SDL_RWFromFile(filePath.c_str(), "r+b");
	if (mapData == nullptr)
	{
		printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());
		openMap(); //Will be handled better in the future, for now, just load the default map
		return false;
	}
	return openMapData(mapData);
}

bool Model::openMap(const std::vector<byte>* bytes)
{
	SDL_RWops* mapData = SDL_RWFromConstMem(bytes, sizeof(bytes));
	if (mapData == nullptr)
	{
		printf("Warning: Unable to read binary data! SDL Error: %s\n", SDL_GetError());
		openMap(); //Will be handled better in the future, for now, just load the default map
		SDL_RWclose(mapData);
		return false;
	}
	return openMapData(mapData);
}

bool Model::openMapData(SDL_RWops* mapData)
{
	bool success = true;
	std::vector<std::tuple<int, int, int, int, char>> tempSwitchInfo;

	//Load data
	printf("Reading file...!\n");
	SDL_RWread(mapData, &mapRows, sizeof(Uint8), 1); //read number of rows of saved map
	SDL_RWread(mapData, &mapCols, sizeof(Uint8), 1); //read number of cols of saved map

	tileMap.reserve(mapRows * mapCols);
	for (int r = 0; r < mapRows; r++)
	{
		for (int c = 0; c < mapCols; c++)
		{
			Uint16 tempType = -1;
			SDL_RWread(mapData, &tempType, sizeof(Uint16), 1);

			if (tempType == DOOR || tempType == LADDER)
			{
				bool tempLocked;
				Uint8 tempDirection = -1;
				SDL_RWread(mapData, &tempLocked, sizeof(bool), 1);
				SDL_RWread(mapData, &tempDirection, sizeof(Uint8), 1);
				exit = new ExitTile(c * tileSize, r * tileSize, tileSize, tempType, false, false, setTileEffect(tempType), tempLocked, tempDirection);
				tileMap.push_back(exit);
			}
			else if (tempType == SWITCH)
			{
				tileMap.push_back(new Switch(c * tileSize, r * tileSize, tileSize));
				char temp;
				SDL_RWread(mapData, &temp, sizeof(char), 1);
				while (temp != '|')
				{
					if (temp == 'T')
					{
						Uint8 tempRow, tempCol;
						SDL_RWread(mapData, &tempRow, sizeof(Uint8), 1);
						SDL_RWread(mapData, &tempCol, sizeof(Uint8), 1);
						tempSwitchInfo.insert(tempSwitchInfo.end(), std::make_tuple(r, c, tempRow, tempCol, temp));
					}
					else if (temp == 'E')
					{

					}
					SDL_RWread(mapData, &temp, sizeof(char), 1);
				}
			}
			else
			{
				tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, tempType, setIsSolid(tempType), ((tempType == PIT) ? true : false), setTileEffect(tempType)));
			}
		}
	}
	int numEntities = 0;
	SDL_RWread(mapData, &numEntities, sizeof(Uint8), 1);
	for (int i = 1; i <= numEntities; i++)
	{
		char entityType;
		Uint8 height, width, velX, velY, hp;
		Uint16 posX, posY;
		SDL_RWread(mapData, &entityType, sizeof(char), 1);
		Entity* anEntity;
		switch (entityType)
		{
		case 'P':
			SDL_RWread(mapData, &height, sizeof(Uint8), 1);
			SDL_RWread(mapData, &width, sizeof(Uint8), 1);
			SDL_RWread(mapData, &posX, sizeof(Uint16), 1);
			SDL_RWread(mapData, &posY, sizeof(Uint16), 1);
			SDL_RWread(mapData, &velX, sizeof(Uint8), 1);
			SDL_RWread(mapData, &velY, sizeof(Uint8), 1);
			SDL_RWread(mapData, &hp, sizeof(Uint8), 1);
			player = new Player(height, width, posX, posY, velX, velY, hp);
			addEntityFromFile(player);
			break;
		case 'A':
			SDL_RWread(mapData, &height, sizeof(Uint8), 1);
			SDL_RWread(mapData, &width, sizeof(Uint8), 1);
			SDL_RWread(mapData, &posX, sizeof(Uint16), 1);
			SDL_RWread(mapData, &posY, sizeof(Uint16), 1);
			SDL_RWread(mapData, &velX, sizeof(Uint8), 1);
			SDL_RWread(mapData, &velY, sizeof(Uint8), 1);
			anEntity = new Arrow(height, width, posX, posY, velX, velY);
			addEntityFromFile(anEntity);
			break;
		case 'E':
			SDL_RWread(mapData, &height, sizeof(Uint8), 1);
			SDL_RWread(mapData, &width, sizeof(Uint8), 1);
			SDL_RWread(mapData, &posX, sizeof(Uint16), 1);
			SDL_RWread(mapData, &posY, sizeof(Uint16), 1);
			SDL_RWread(mapData, &velX, sizeof(Uint8), 1);
			SDL_RWread(mapData, &velY, sizeof(Uint8), 1);
			SDL_RWread(mapData, &hp, sizeof(Uint8), 1);
			anEntity = new Enemy(height, width, posX, posY, velX, velY, hp);
			addEntityFromFile(anEntity);
			break;
		default:
			SDL_RWclose(mapData);
			printf("Error: Encountered unexpected file input of entity type %c \n", entityType);
			return false;
			break;
		}
	}
	//Close file handler
	SDL_RWclose(mapData);

	if (tempSwitchInfo.size() > 0)
	{
		for (int i = 0; i < tempSwitchInfo.size(); i++)
		{
			Switch* currentSwitch = dynamic_cast<Switch*>(getTileAtMapIndex(std::get<0>(tempSwitchInfo[i]), std::get<1>(tempSwitchInfo[i])));
			if (std::get<4>(tempSwitchInfo[i]) == 'T')
			{
				currentSwitch->addToggleable(dynamic_cast<Toggleable*>(getTileAtMapIndex(std::get<2>(tempSwitchInfo[i]), std::get<3>(tempSwitchInfo[i]))));
			}
			else if (std::get<4>(tempSwitchInfo[i]) == 'E')
			{

			}
		}
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

	SDL_RWops* mapData = SDL_RWFromFile(filePath.c_str(), "w+b");
	SDL_RWwrite(mapData, &mapRows, sizeof(Uint8), 1);
	SDL_RWwrite(mapData, &mapCols, sizeof(Uint8), 1);
	if (mapData != nullptr)
	{
		//Save data
		for (int r = 0; r < mapRows; r++)
		{
			for (int c = 0; c < mapCols; c++)
			{
				Uint16 tempType = tileMap[(r*mapCols) + c]->getType();
				if (tempType == DOOR || tempType == LADDER)
				{
					bool tempLocked = dynamic_cast<ExitTile*>(tileMap[(r*mapCols) + c])->getIsLocked();
					Sint32 tempDirection = dynamic_cast<ExitTile*>(tileMap[(r*mapCols) + c])->getExitDirection();
					SDL_RWwrite(mapData, &tempType, sizeof(Uint16), 1);
					SDL_RWwrite(mapData, &tempLocked, sizeof(bool), 1);
					SDL_RWwrite(mapData, &tempDirection, sizeof(Uint8), 1);
				}
				else if (tempType == SWITCH || tempType == SWITCH_WEIGHTED || tempType == SWITCH_LEVER)
				{
					SDL_RWwrite(mapData, &tempType, sizeof(Uint16), 1);
					char temp;
					for (Toggleable* t : dynamic_cast<Switch*>(tileMap[(r*mapCols) + c])->getConnectedToggleables())
					{
						Tile *aTile = dynamic_cast<Tile*>(t);
						if (aTile != nullptr)
						{
							temp = 'T';
							SDL_RWwrite(mapData, &temp, sizeof(char), 1);
							Uint8 tempInt = aTile->getRow();
							SDL_RWwrite(mapData, &tempInt, sizeof(Uint8), 1);
							tempInt = aTile->getCol();
							SDL_RWwrite(mapData, &tempInt, sizeof(Uint8), 1);
							continue;
						}
						Entity *anEntity = dynamic_cast<Entity*>(t);
						if (anEntity != nullptr)
						{

						}
					}
					temp = '|'; //signifies end of toggleables vector
					SDL_RWwrite(mapData, &temp, sizeof(char), 1);
				}
				else
				{
					SDL_RWwrite(mapData, &tempType, sizeof(Uint16), 1);
				}
			}
		}
		char temp = 'P'; //player
		int tempInt = getNumberOfEntities();
		SDL_RWwrite(mapData, &tempInt, sizeof(Uint8), 1); //number of entities
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
			SDL_RWwrite(mapData, &temp, sizeof(char), 1);
			tempInt = e->getHeight();
			SDL_RWwrite(mapData, &tempInt, sizeof(Uint8), 1);
			tempInt = e->getWidth();
			SDL_RWwrite(mapData, &tempInt, sizeof(Uint8), 1);
			tempInt = e->getPosX();
			SDL_RWwrite(mapData, &tempInt, sizeof(Uint16), 1);
			tempInt = e->getPosY();
			SDL_RWwrite(mapData, &tempInt, sizeof(Uint16), 1);
			tempInt = e->getVelocityX();
			SDL_RWwrite(mapData, &tempInt, sizeof(Uint8), 1);
			tempInt = e->getVelocityY();
			SDL_RWwrite(mapData, &tempInt, sizeof(Uint8), 1);
			if (temp != 'A')
			{
				tempInt = e->getHealth();
				SDL_RWwrite(mapData, &tempInt, sizeof(Uint8), 1);
			}
		}
		//Close file handler
		SDL_RWclose(mapData);
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
