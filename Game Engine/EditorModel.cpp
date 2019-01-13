#include "EditorModel.h"
#include <SDL.h>
#undef main
#include <iostream>


EditorModel::EditorModel()
{
	tileSize = 32;
	Model::openMap("blankMap"); //temp
	levelHeight = mapRows * tileSize;
	levelWidth = mapCols * tileSize;
}

EditorModel::~EditorModel()
{
}

void EditorModel::setSelectedTileType(int newType)
{
	if (newType >= 0 && newType < NUMBER_TILE_TYPES + 3)
	{
		selectedTileType = newType;
	}
}

int EditorModel::getSelectedTileType()
{
	return selectedTileType;
}

void EditorModel::clickTile(int x, int y)
{
	int col = x / tileSize;
	int row = y / tileSize;
	if (col < 0 || row < 0)
	{
		return;
	}
	switch (selectedTileType)
	{
	case 18:
	{
		if (getTileAtMapIndex(row, col)->isSolid() || getTileAtMapIndex(row, col)->getType() == PIT)
		{
			break;
		}
		if (player == nullptr)
		{
			player = new Player(tileSize, tileSize, col * tileSize, row * tileSize, 0, 0, 10);
			for (Entity* e : entities)
			{
				if (SDL_HasIntersection(e->getGroundHitBox(), player->getGroundHitBox()))
				{
					removeEntity(e);
					break;
				}
			}
			entities.insert(entities.begin(), player);
			//addEntity(player);
		}
		else
		{
			player->setPosX(col * tileSize);
			player->setPosY(row * tileSize);
			for (Entity* e : entities)
			{
				if (SDL_HasIntersection(e->getGroundHitBox(), player->getGroundHitBox()) && e != player)
				{
					removeEntity(e);
					break;
				}
			}
		}
		break;
	}
	case 19:
	{
		if (getTileAtMapIndex(row, col)->isSolid() || getTileAtMapIndex(row, col)->getType() == PIT)
		{
			break;
		}
		Enemy* enemy = new Enemy(tileSize, tileSize, col * tileSize, row * tileSize, 0, 0, 3);
		for (Entity* e : entities)
		{
			if (SDL_HasIntersection(e->getGroundHitBox(), enemy->getGroundHitBox()))
			{
				if (e == player)
				{
					player = nullptr;
				}
				removeEntity(e);
				break;
			}
		}
		addEntity(enemy);
		break;
	}
	case 20:
	{
		if (getTileAtMapIndex(row, col)->isSolid())
		{
			break;
		}
		Arrow* arrow = new Arrow(tileSize, tileSize, col*tileSize, row*tileSize, 0, 0);
		for (Entity* e : entities)
		{
			if (SDL_HasIntersection(e->getGroundHitBox(), arrow->getGroundHitBox()))
			{
				if (e == player)
				{
					player = nullptr;
				}
				removeEntity(e);
				break;
			}
		}
		addEntity(arrow);
		break;
	}
	default:
	{
		Tile* newTile = new Tile(col * tileSize, row * tileSize, tileSize, selectedTileType, setIsSolid(selectedTileType), ((selectedTileType == PIT) ? true : false), nullptr);
		replaceTile(row, col, newTile);
		if (setIsSolid(selectedTileType) || ((selectedTileType == PIT) ? true : false))
		{
			for (Entity* e : entities)
			{
				if (SDL_HasIntersection(e->getGroundHitBox(), newTile->getTileSpace()))
				{
					if (e == player)
					{
						player = nullptr;
					}
					removeEntity(e);
				}
			}
		}
		break;
	}
	}
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
			tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, testMap[r][c], setIsSolid(testMap[r][c]), ((testMap[r][c] == PIT) ? true : false), nullptr));
		}
	}
	saveMap("blankMap");
	return success;
}

void EditorModel::replaceTile(int row, int col, Tile * newTile)
{
	Tile* temp = getTileAtMapIndex(row, col);
	tileMap.at((row*mapCols) + col) = newTile;
	delete temp;
}
