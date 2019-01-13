#include "GameModel.h"
#include <SDL.h>
#undef main
#include <iostream>
#include "Model.h"

GameModel::GameModel()
{
	tileSize = 32;
	Model::openMap("testMap"); //temp
	levelHeight = mapRows * tileSize;
	levelWidth = mapCols * tileSize;
}

GameModel::GameModel(std::vector<Tile*> map, int rows, int cols, std::vector<Entity*> levelEntities)
{
	mapRows = rows;
	mapCols = cols;
	tileSize = 32; 
	for (Tile* t : map)
	{
		tileMap.insert(tileMap.end(), t->clone());
		tileMap.back()->setTileEffect(setTileEffect(t->getType()));
	}
	for (auto* e : levelEntities)
	{
		addEntity(e->clone());
	}
	player = dynamic_cast<Player*>(entities[0]);
	for (auto* e : entities)
	{
		entitiesInitialState.insert(entitiesInitialState.end(), e->clone());
	}
	levelHeight = mapRows * tileSize;
	levelWidth = mapCols * tileSize;
}


GameModel::~GameModel()
{
	delete downTreadmillEffect;
	delete rightTreadmillEffect;
	delete leftTreadmillEffect;
	delete upTreadmillEffect;
	delete iceEffect;
	delete mudEffect;
	delete lavaEffect;
	delete spikeEffect;
	for (Entity* e : entitiesInitialState)
	{
		delete e;
	}
}

void GameModel::moveEntities(double delta)
{
	//printf("Player x: %f player y: %f \n", player->getPosX(), player->getPosY());
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

bool GameModel::getIsGameOver()
{
	return isGameOver;
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

	TileEffect* slideIntoPitEffect = nullptr;
	bool isOverAPit = false;
	float pitSlideX = 0;
	float pitSlideY = 0;
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
					TileEffect* anEffect = current->getTileEffect();
					bool isUnique = true;
					if (current->isAPit())
					{
						isOverAPit = true;

						float vectorX = current->getCenterPosX() - e->getGroundCenterPosX();
						float vectorY = current->getCenterPosY() - e->getGroundCenterPosY();
						double magnitude = sqrt(pow(vectorX, 2) + pow(vectorY, 2));
						//get unit vector:
						vectorX /= magnitude;
						vectorY /= magnitude;
						pitSlideX += vectorX;
						pitSlideY += vectorY;
					}
					else
					{
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
	}
	if (isOverAPit)
	{
		pitSlideX *= 0.3;
		pitSlideY *= 0.3;
		slideIntoPitEffect = new TileEffect(1, 1, pitSlideX, pitSlideY);
		effects.insert(effects.begin(), slideIntoPitEffect);
	}
	e->determineMovement(player->getCenterPosX(), player->getCenterPosY(), effects);
	delete slideIntoPitEffect;
	if (isCompletelyOverPit(e, posRowTop, posRowBottom, posColLeft, posColRight))
	{
		fallIntoPit(e);
		return;
	}
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

bool GameModel::isCompletelyOverPit(Entity * entity, int topRow, int bottomRow, int leftCol, int rightCol)
{
	for (int r = topRow; r <= bottomRow; r++)
	{
		for (int c = leftCol; c <= rightCol; c++)
		{
			if (r < mapRows && r >= 0 && c >= 0 && c < mapCols && SDL_HasIntersection(entity->getGroundHitBox(), getTileAtMapIndex(r, c)->getTileSpace()))
			{
				if (!getTileAtMapIndex(r, c)->isAPit())
				{
					return false;
				}
			}
		}
	}
	return true;
}

void GameModel::fallIntoPit(Entity * e)
{
	e->takeDamage(1);
	if (e == player)
	{
		e->setPosX(entitiesInitialState[0]->getPosX());
		e->setPosY(entitiesInitialState[0]->getPosY());
		e->setVelocityX(0);
		e->setVelocityY(0);
	}
	else
	{
		killEntity(e);
	}
}

void GameModel::killEntity(Entity * e)
{
	if (e == getPlayer())
	{
		isGameOver = true;
		return;
	}
	removeEntity(e);
}

bool GameModel::openMap()
{
	bool success = true;
	mapRows = 20;
    mapCols = 30;
	int testMap[20][30] = { { 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 12, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 15, 15, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 15, 15, 15, 15, 15, 15, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 15, 0, 15, 0, 0, 0, 0, 8, 8, 0, 8, 8, 0, 0, 0, 0, 17, 0, 0, 12, 0, 6 },
							{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
							{ 6, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 12, 0, 6 },
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
			tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, testMap[r][c], setIsSolid(testMap[r][c]), ((testMap[r][c] == PIT) ? true : false), setTileEffect(testMap[r][c])));
		}
	}
	player = new Player(32, 32, 60, 80, 0, 0, 10);
	Arrow* arrow = new Arrow(32, 32, 300, 80, -5, 0);
	Enemy* enemy1 = new Enemy(50, 40, 70, 300, 0, 0, 1);
	Enemy* enemy2 = new Enemy(20, 20, 230, 255, 0, 0, 1);
	addEntity(player);
	addEntity(arrow);
	addEntity(enemy1);
	addEntity(enemy2);
	entitiesInitialState.insert(entitiesInitialState.end(), new Player(32, 32, 60, 80, 0, 0, 10));
	entitiesInitialState.insert(entitiesInitialState.end(), new Arrow(32, 32, 300, 80, -5, 0));
	entitiesInitialState.insert(entitiesInitialState.end(), new Enemy(50, 40, 70, 300, 0, 0, 1));
	entitiesInitialState.insert(entitiesInitialState.end(), new Enemy(20, 20, 230, 255, 0, 0, 1));
	saveMap("testMap");
	return success;
}

void GameModel::addEntityFromFile(Entity * e)
{
	addEntity(e);
	entitiesInitialState.insert(entitiesInitialState.end(), e->clone());
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
