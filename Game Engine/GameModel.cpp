#include "GameModel.h"
#include "Rectangle.h"


GameModel::GameModel()
{
	player = new Rectangle(10, 10, 20, 50, 0, 0);
}


GameModel::~GameModel()
{
}

void GameModel::setAreaWidth(int width)
{
	gameAreaWidth = width;
}

void GameModel::setAreaHeight(int height)
{
	gameAreaHeight = height;
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

void GameModel::moveAnEntity(Entity * e, double delta) const
{
	e->setPosX(e->getPosX() + (e->getVelocityX() * delta));

	if (e->getPosX() <= 0) //If movement puts past the left bound of screen, put it against the left bound instead and set velocity to zero
	{
		e->setPosX(0);
		e->setVelocityX(0);
	}
	else if ((e->getPosX() + e->getWidth()) >= gameAreaWidth) //If movement puts past the right bound of screen, put it against the right bound instead and set velocity to zero
	{
		e->setPosX(gameAreaWidth - e->getWidth());
		e->setVelocityX(0);
	}

	e->setPosY(e->getPosY() + (e->getVelocityY() * delta));

	if (e->getPosY() <= 0) //If movement puts past the top bound of screen, put it against the top bound instead and set velocity to zero
	{
		e->setPosY(0);
		e->setVelocityY(0);
	}
	else if ((e->getPosY() + e->getHeight()) >= gameAreaHeight) //If movement puts past the bottom bound of screen, put it against the bottom bound instead and set velocity to zero
	{
		e->setPosY(gameAreaHeight - e->getHeight());
		e->setVelocityY(0);
	}
}
