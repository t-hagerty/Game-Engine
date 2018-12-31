#include "EditorModel.h"



EditorModel::EditorModel()
{
	tileSize = 32;
	openMap("blankMap");
	levelHeight = mapRows * tileSize;
	levelWidth = mapCols * tileSize;
}


EditorModel::~EditorModel()
{
	deleteMap();
}

void EditorModel::addEntity(Entity * newEntity)
{
	entities.insert(entities.end(), newEntity);
}

Entity * EditorModel::getEntity(int index)
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

void EditorModel::removeEntity(Entity * e)
{
	for (int i = 0; i < static_cast<int>(entities.size()); i++)
	{
		if (e == getEntity(i))
		{
			entities.erase(entities.begin() + i);
			return;
		}
	}
}

int EditorModel::getNumberOfEntities() const
{
	return entities.size();
}

int EditorModel::getTileSize() const
{
	return tileSize;
}

int EditorModel::getMapRows() const
{
	return mapRows;
}

int EditorModel::getMapCols() const
{
	return mapCols;
}

int EditorModel::getLevelWidth() const
{
	return levelWidth;
}

int EditorModel::getLevelHeight() const
{
	return levelHeight;
}

void EditorModel::setSelectedTileType(int newType)
{
	if (newType >= 0 && newType < NUMBER_TILE_TYPES)
	{
		selectedTileType = newType;
	}
}

int EditorModel::getSelectedTileType()
{
	return selectedTileType;
}

std::vector<Tile*> EditorModel::getTileMap() const
{
	return tileMap;
}

void EditorModel::clickTile(int x, int y)
{
	int col = x / tileSize;
	int row = y / tileSize;
	replaceTile(row, col, new Tile(col * tileSize, row * tileSize, tileSize, selectedTileType, setIsSolid(selectedTileType), ((selectedTileType == 15) ? true : false), nullptr));
}

bool EditorModel::openMap()
{
	bool success = true;
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
	tileMap.reserve(mapRows * mapCols);
	for (int r = 0; r < mapRows; r++)
	{
		for (int c = 0; c < mapCols; c++)
		{
			tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, testMap[r][c], setIsSolid(testMap[r][c]), ((testMap[r][c] == 15) ? true : false), nullptr));
		}
	}
	saveMap("blankMap");
	return success;
}

bool EditorModel::openMap(std::string filePath)
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
				bool isPit = false;
				if (tempType == 15)
				{
					isPit = true;
				}
				tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, tempType, setIsSolid(tempType), isPit, nullptr));
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
				addEntity(player);
				break;
			case 'A':
				SDL_RWread(file, &height, sizeof(Sint32), 1);
				SDL_RWread(file, &width, sizeof(Sint32), 1);
				SDL_RWread(file, &posX, sizeof(Sint32), 1);
				SDL_RWread(file, &posY, sizeof(Sint32), 1);
				SDL_RWread(file, &velX, sizeof(Sint32), 1);
				SDL_RWread(file, &velY, sizeof(Sint32), 1);
				anEntity = new Arrow(height, width, posX, posY, velX, velY);
				addEntity(anEntity);
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
				addEntity(anEntity);
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

bool EditorModel::saveMap(std::string filePath) const
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
				SDL_RWwrite(file, &tempType, sizeof(Sint32), 1);
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

Tile * EditorModel::getTileAtMapIndex(int row, int col) const
{
	return tileMap.at((row*mapCols) + col);
}

void EditorModel::replaceTile(int row, int col, Tile * newTile)
{
	Tile* temp = getTileAtMapIndex(row, col);
	tileMap.at((row*mapCols) + col) = newTile;
	delete temp;
}

bool EditorModel::setIsSolid(int tileType)
{
	return isSolidTable[tileType];
}

bool EditorModel::isInsideWall(Entity * entity, Tile * t)
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

bool EditorModel::isInsideAnyWalls(Entity * entity, int topRow, int bottomRow, int leftCol, int rightCol) const
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

bool EditorModel::isIntersectingEntity(Entity * e1, Entity * e2)
{
	if (SDL_HasIntersection(e1->getCollisionBox(), e2->getCollisionBox()))
	{
		return true;
	}
	return false;
}

void EditorModel::deleteMap() const
{
	for (int i = 0; i < mapRows*mapCols; ++i)
	{
		delete tileMap[i];
	}
}
