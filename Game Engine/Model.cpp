#include "Model.h"
#include <tuple>
#include <sstream>
#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/ws_client.h>    

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;
using namespace web::json;

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

template< typename T > void toByteArray(const T& object, std::vector<byte>* bytes)
{
	std::vector<byte> newBytes;

	const byte* begin = reinterpret_cast<const byte*>(std::addressof(object));
	const byte* end = begin + sizeof(T);
	std::copy(begin, end, std::back_inserter(newBytes));
	std::copy(newBytes.begin(), newBytes.end(), std::back_inserter(*bytes));
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
	SDL_RWops* mapData = SDL_RWFromFile(filePath.c_str(), "w+b");

	if (mapData == nullptr)
	{
		printf("Error: Unable to save file! %s\n", SDL_GetError());
		return false;
	}
	std::vector<byte>* mapBinary = levelToBinary();

	for (int i = 0; i < static_cast<int>(mapBinary->size()); i++)
	{
		SDL_RWwrite(mapData, &mapBinary->at(i), sizeof(char), 1);
	}

	return true;
}

bool Model::publishMap() const
{
	printf("Publish to database");
	std::vector<unsigned char>* level = levelToBinary();
	utility::string_t encodedLevel = utility::conversions::to_base64(*level);
	std::string s = utility::conversions::to_utf8string(encodedLevel);
	std::cout << s;

	http_client client(U("http://localhost:8080"));
	pplx::task<http_response> resp = client.request(methods::GET, U("/users/1"));

	resp.then([=](pplx::task<web::http::http_response> task)
	{
		web::http::http_response  response = task.get();
		if (response.status_code() == status_codes::OK)
		{
			return response.extract_json();
		}
		return pplx::task_from_result(json::value());
	})
	.then([](pplx::task<json::value> previousTask)
	{
		try
		{
			json::value jvalue = previousTask.get();
			if (!jvalue.is_null())
			{
				std::string result = utility::conversions::to_utf8string(jvalue.serialize());
				std::cout << result;
			}
		}
		catch (http_exception const & e)
		{
			printf(e.what());
			printf("\n");
		}
	})
	.wait();
	return true;
}

std::vector<byte>* Model::levelToBinary() const
{
	std::vector<byte>* bytes = new std::vector<byte>();

	toByteArray((Uint8)mapRows, bytes);
	toByteArray((Uint8)mapCols, bytes);
	for (int r = 0; r < mapRows; r++)
	{
		for (int c = 0; c < mapCols; c++)
		{
			Uint16 tempType = tileMap[(r*mapCols) + c]->getType();
			if (tempType == DOOR || tempType == LADDER)
			{
				bool tempLocked = dynamic_cast<ExitTile*>(tileMap[(r*mapCols) + c])->getIsLocked();
				Uint8 tempDirection = dynamic_cast<ExitTile*>(tileMap[(r*mapCols) + c])->getExitDirection();
				toByteArray(tempType, bytes);
				toByteArray(tempLocked, bytes);
				toByteArray(tempDirection, bytes);
			}
			else if (tempType == SWITCH || tempType == SWITCH_WEIGHTED || tempType == SWITCH_LEVER)
			{
				toByteArray(tempType, bytes);
				char temp;
				for (Toggleable* t : dynamic_cast<Switch*>(tileMap[(r*mapCols) + c])->getConnectedToggleables())
				{
					Tile *aTile = dynamic_cast<Tile*>(t);
					if (aTile != nullptr)
					{
						temp = 'T';
						toByteArray(temp, bytes);
						Uint8 tempInt = aTile->getRow();
						toByteArray(tempInt, bytes);
						tempInt = aTile->getCol();
						toByteArray(tempInt, bytes);
						continue;
					}
					Entity *anEntity = dynamic_cast<Entity*>(t);
					if (anEntity != nullptr)
					{

					}
				}
				temp = '|'; //signifies end of toggleables vector
				toByteArray(temp, bytes);
			}
			else
			{
				toByteArray(tempType, bytes);
			}
		}
	}
	char temp = 'P'; //player
	Uint8 tempInt = getNumberOfEntities();
	Uint16 tempInt16;
	toByteArray(tempInt, bytes);
	for (int i = 0; i < static_cast<int>(entities.size()); i++)
	{
		Entity* e = entities[i];
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
		toByteArray(temp, bytes);
		tempInt = e->getHeight();
		toByteArray(tempInt, bytes);
		tempInt = e->getWidth();
		toByteArray(tempInt, bytes);
		tempInt16 = e->getPosX();
		toByteArray(tempInt16, bytes);
		tempInt16 = e->getPosY();
		toByteArray(tempInt16, bytes);
		tempInt = e->getVelocityX();
		toByteArray(tempInt, bytes);
		tempInt = e->getVelocityY();
		toByteArray(tempInt, bytes);
		if (temp != 'A')
		{
			tempInt = e->getHealth();
			toByteArray(tempInt, bytes);
		}
	}

	return bytes;
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
