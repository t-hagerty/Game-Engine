#include "GameModel.h"
#include <SDL.h>
#undef main
#include <iostream>
#include "Player.h"
#include "Projectile.h"
#include "Arrow.h"
#include "Enemy.h"


GameModel::GameModel()
{
	tileSize = 32; //TODO change later so that this size adjusts based on the size of the screen/window
	openMap("testMap");
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
		if (e->getHealth() <= 0)
		{
			killEntity(e);
			continue;
		}
		moveAnEntity(e, delta);
	}
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

void GameModel::removeEntity(Entity * e)
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

int GameModel::getNumberOfEntities() const
{
	return entities.size();
}

Player * GameModel::getPlayer() const
{
	return player;
}

bool GameModel::getIsGameOver()
{
	return isGameOver;
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

TileEffect * GameModel::setTileEffect(int tileType)
{
	switch (tileType)
	{
	case DOWN_TREADMILL:
		return downTreadmillEffect;
	case RIGHT_TREADMILL:
		return rightTreadmillEffect;
	case LEFT_TREADMILL:
		return leftTreadmillEffect;
	case UP_TREADMILL:
		return upTreadmillEffect;
	case ICE:
		return iceEffect;
	case MUD:
		return mudEffect;
	case LAVA:
		return lavaEffect;
	case SPIKES:
		return spikeEffect;
	default:
		return nullptr;
	}
}

void GameModel::moveAnEntity(Entity * e, double delta)
{
	double oldX = e->getPosX();
	double oldY = e->getPosY();
	int posColLeft = oldX / tileSize;
	int posColRight = (oldX + e->getWidth()) / tileSize;
	int posRowTop = oldY / tileSize;
	int posRowBottom = (oldY + e->getHeight()) / tileSize;

	e->decrementTimers(delta);

	std::vector<TileEffect*> effects = {};
	for (int r = posRowTop; r <= posRowBottom; r++)
	{
		for (int c = posColLeft; c <= posColRight; c++)
		{
			if (r < mapRows && c < mapCols && r >= 0 && c >= 0)
			{
				if (SDL_HasIntersection(getTileAtMapIndex(r, c)->getTileSpace(), e->getGroundHitBox()))
				{
					Tile* current = getTileAtMapIndex(r, c);
					TileEffect* anEffect = current->getMovementEffect();
					bool isUnique = true;
					for (TileEffect* effect : effects)
					{
						if (anEffect == effect)
						{
							isUnique = false;
						}
					}
					if (anEffect != nullptr && isUnique)
					{
						effects.insert(effects.begin(), anEffect);
						if (anEffect->getDamage() > 0 && e->getInvulnTimer() == 0)
						{
							e->takeDamage(anEffect->getDamage());
							e->beKnockedBack(current->getCenterPosX(), current->getCenterPosY(), anEffect->getKnockbackForce());
						}
					}
				}
			}
		}
	}
	e->determineMovement(player->getCenterPosX(), player->getCenterPosY(), effects);
	if (e->getVelocityY() != 0)
	{
		//=== Y MOVEMENT (UP(neg)/DOWN(pos)) ===
		e->setPosY(e->getPosY() + (e->getVelocityY() * delta));
		double newY = e->getPosY();
		posRowTop = newY / tileSize;
		posRowBottom = (newY + e->getHeight()) / tileSize;

		if (e->getPosY() <= 0) //If movement puts past the top bound of screen, put it against the top bound instead and set velocity to zero
		{
			e->setPosY(0);
			e->setVelocityY(0);
			posRowTop = 0;
		}
		else if ((e->getPosY() + e->getHeight()) >= levelHeight) //If movement puts past the bottom bound of screen, put it against the bottom bound instead and set velocity to zero
		{
			e->setPosY(levelHeight - e->getHeight());
			e->setVelocityY(0);
			posRowBottom = mapRows - 1;
		}
		if (e->getVelocityY() > 0) //down
		{
			for (Entity* anotherEntity : entities)
			{
				if (e != anotherEntity && isIntersectingEntity(e, anotherEntity))
				{
					e->setPosY(anotherEntity->getPosY() - e->getHeight());
					e->collideWithEntity(anotherEntity);
				}
			}
			if ((posRowBottom) < mapRows && posColRight < mapCols && isInsideAnyWalls(e, posRowTop, posRowBottom, posColLeft, posColRight))
			{
				e->hitWall(0);
				e->setPosY((posRowBottom)* tileSize - e->getHeight());
			}
		}
		else if (e->getVelocityY() < 0) //up
		{
			for (Entity* anotherEntity : entities)
			{
				if (e != anotherEntity && isIntersectingEntity(e, anotherEntity))
				{
					e->setPosY(anotherEntity->getPosY() + anotherEntity->getHeight());
					e->collideWithEntity(anotherEntity);
				}
			}
			if ((posRowTop) >= 0 && posColRight < mapCols && isInsideAnyWalls(e, posRowTop, posRowBottom, posColLeft, posColRight))
			{
				e->hitWall(3);
				e->setPosY((posRowTop + 1)* tileSize - (e->getHeight() - e->getGroundHitBox()->h));
			}
		}
	}
	if(e->getVelocityX() != 0)
	{
		//=== X MOVEMENT (LEFT(neg)/RIGHT(pos)) ===
		e->setPosX(e->getPosX() + (e->getVelocityX() * delta));
		double newX = e->getPosX();
		posColLeft = newX / tileSize;
		posColRight = (newX + e->getWidth()) / tileSize;

		if (e->getPosX() <= 0) //If movement puts past the left bound of screen, put it against the left bound instead and set velocity to zero
		{
			e->setPosX(0);
			e->setVelocityX(0);
			posColLeft = 0;
		}
		else if ((e->getPosX() + e->getWidth()) >= levelWidth) //If movement puts past the right bound of screen, put it against the right bound instead and set velocity to zero
		{
			e->setPosX(levelWidth - e->getWidth());
			e->setVelocityX(0);
			posColRight = mapCols - 1;
		}

		if (e->getVelocityX() > 0) //right
		{
			for (Entity* anotherEntity : entities)
			{
				if (e != anotherEntity && isIntersectingEntity(e, anotherEntity))
				{
					e->setPosX(anotherEntity->getPosX() - e->getWidth());
					e->collideWithEntity(anotherEntity);
				}
			}
			if ((posColRight) < mapCols && posRowBottom < mapRows && isInsideAnyWalls(e, posRowTop, posRowBottom, posColLeft, posColRight))
			{
				e->hitWall(1);
				e->setPosX((posColRight)* tileSize - e->getWidth());
			}
		}
		else if (e->getVelocityX() < 0) //left
		{
			for (Entity* anotherEntity : entities)
			{
				if (e != anotherEntity && isIntersectingEntity(e, anotherEntity))
				{
					e->setPosX(anotherEntity->getPosX() + anotherEntity->getWidth());
					e->collideWithEntity(anotherEntity);
				}
			}
			if ((posColLeft) >= 0 && posRowBottom < mapRows && isInsideAnyWalls(e, posRowTop, posRowBottom, posColLeft, posColRight)) //left
			{
				e->hitWall(2);
				e->setPosX((posColLeft + 1)* tileSize);
			}
		}
	}
	//Safety check, make sure once we're done, the entity didnt manage to still make it inside a wall, if so, fallback on moving it back to former pos:
	if(isInsideAnyWalls(e, posRowTop, posRowBottom, posColLeft, posColRight))
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
			e->collideWithEntity(anotherEntity);
		}
	}
}

void GameModel::knockback(Entity * knockerbacker, Entity * knockedback)
{
	//Calculate knockback:
	float vectorX = knockerbacker->getCenterPosX() - knockedback->getCenterPosX();
	float vectorY = knockerbacker->getCenterPosY() - knockedback->getCenterPosY();
	double magnitude = sqrt(pow(vectorX, 2) + pow(vectorY, 2));
	//get unit vector:
	vectorX /= magnitude;
	vectorY /= magnitude;
	//multiply vector by velocity of enemy:
	float knockbackForce = knockerbacker->getKnockbackForce() * -1;
	vectorX *= knockbackForce;
	vectorY *= knockbackForce;
	knockedback->setVelocityX(vectorX);
	knockedback->setVelocityY(vectorY);
}

bool GameModel::isInsideWall(Entity* entity, Tile * t)
{
	if(!t->isSolid())
	{
		return false;
	}
	if(SDL_HasIntersection(t->getTileSpace(), entity->getGroundHitBox()))
	{
		return true;
	}
	return false;
}

bool GameModel::isInsideAnyWalls(Entity * entity, int topRow, int bottomRow, int leftCol, int rightCol) const
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

bool GameModel::isIntersectingEntity(Entity* e1, Entity* e2)
{
	if (SDL_HasIntersection(e1->getCollisionBox(), e2->getCollisionBox()))
	{
		return true;
	}
	return false;
}

void GameModel::killEntity(Entity * e)
{
	if (e == getPlayer())
	{
		isGameOver = true;
		return;
	}
	removeEntity(e);
	delete e;
}

bool GameModel::openMap()
{
	bool success = true;
	mapRows = 20;
    mapCols = 30;
	int testMap[20][30] = { { 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 12, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 8, 8, 0, 0, 0, 0, 17, 0, 0, 12, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
							{ 6, 0, 9, 0, 9, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 17, 17, 0, 0, 12, 0, 6 },
							{ 6, 0, 9, 0, 9, 0, 9, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
							{ 0, 0, 9, 0, 9, 0, 9, 0, 8, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 },
							{ 0, 0, 13, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 16, 16, 16, 0, 0, 12, 0, 0 },
							{ 0, 0, 13, 0, 0, 0, 12, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 16, 16, 16, 0, 0, 12, 0, 0 },
							{ 11, 11, 0, 0, 12, 0, 12, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 16, 0, 0, 12, 0, 0 },
							{ 6, 0, 9, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
							{ 6, 0, 9, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
							{ 6, 0, 9, 0, 12, 0, 0, 13, 13, 13, 13, 0, 0, 0, 0, 0, 0, 0, 10, 10, 10, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
							{ 6, 0, 14, 14, 14, 0, 0, 13, 13, 13, 13, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 9, 0, 0, 12, 0, 6 },
							{ 6, 0, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 12, 0, 6 },
							{ 6, 0, 14, 14, 0, 0, 10, 10, 11, 11, 0, 9, 0, 0, 13, 14, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 6 },
							{ 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 9, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 2 } };
	tileMap.reserve(mapRows * mapCols);
	for (int r = 0; r < mapRows; r++)
	{
		for(int c = 0; c < mapCols; c++)
		{
			tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, testMap[r][c], setIsSolid(testMap[r][c]), setTileEffect(testMap[r][c])));
		}
	}
	player = new Player(32, 32, 60, 80, 0, 0, 10);
	Arrow* arrow = new Arrow(32, 32, 300, 80, -5, 0);
	Enemy* enemy1 = new Enemy(50, 40, 70, 300, 0, 0, 1);
	Enemy* enemy2 = new Enemy(20, 20, 230, 255, 0, 0, 1);
	addEntity(arrow);
	addEntity(enemy1);
	addEntity(enemy2);
	addEntity(player);
	entitiesInitialState.insert(entitiesInitialState.end(), new Player(32, 32, 60, 80, 0, 0, 10));
	entitiesInitialState.insert(entitiesInitialState.end(), new Arrow(32, 32, 300, 80, -5, 0));
	entitiesInitialState.insert(entitiesInitialState.end(), new Enemy(50, 40, 70, 300, 0, 0, 1));
	entitiesInitialState.insert(entitiesInitialState.end(), new Enemy(20, 20, 230, 255, 0, 0, 1));
	saveMap("testMap");
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
				tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, tempType, setIsSolid(tempType), setTileEffect(tempType)));
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
				entitiesInitialState.insert(entitiesInitialState.end(), new Player(height, width, posX, posY, velX, velY, hp));
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
				entitiesInitialState.insert(entitiesInitialState.end(), new Arrow(height, width, posX, posY, velX, velY));
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
				entitiesInitialState.insert(entitiesInitialState.end(), new Enemy(height, width, posX, posY, velX, velY, hp));
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

bool GameModel::saveMap(std::string filePath) const
{
	//Likely temporary and can be removed once a level editor is implemented
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
				int tempType = tileMap[(r*mapCols) + c]->getType();
				SDL_RWwrite(file, &tempType, sizeof(Sint32), 1);
			}
		}
		/*
		* TEMPORARY MANUALLY WRITING SAVE DATA FOR TEST ENTITIES
		* I know it's awful, but once there's a level editor it'll be deleted anyways so I did it the easy but bad way.
		*/
		char temp = 'P'; //player
		int tempInt = 4;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1); //number of entities
		SDL_RWwrite(file, &temp, sizeof(char), 1);
		tempInt = 32;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 60;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 80;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 0;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 10;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		temp = 'A'; //Arrow projectile
		SDL_RWwrite(file, &temp, sizeof(char), 1);
		tempInt = 32;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 300;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 80;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = -5;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 0;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		temp = 'E'; //enemy
		SDL_RWwrite(file, &temp, sizeof(char), 1);
		tempInt = 50;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 40;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 70;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 300;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 0;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 1;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		SDL_RWwrite(file, &temp, sizeof(char), 1);
		tempInt = 20;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 230;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 255;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 0;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
		tempInt = 1;
		SDL_RWwrite(file, &tempInt, sizeof(Sint32), 1);
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
		delete tileMap[i];
	}
}


void GameModel::resetLevel()
{
	//delete all entities
	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}
	entities.clear();
	//replace all entities to original spots/states
	for (auto* e : entitiesInitialState)
	{
		addEntity(e->clone());
	}
	player = static_cast<Player*>(entities[0]); //According to save file format, player should always end up as first in the array so this is safe-ish
	isGameOver = false;
}
