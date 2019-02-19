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
	case DOOR:
	{
		if (exit != nullptr)
		{
			removeExit();
		}

		short direction = -1;
		//TODO: determine direction of door based on surrounding walls
		ExitTile* newTile = new ExitTile(col * tileSize, row * tileSize, tileSize, selectedTileType, false, false, nullptr, false, direction);
		replaceTile(row, col, newTile);
		exit = newTile;
		break;
	}
	case LADDER:
	{
		if (exit != nullptr)
		{
			removeExit();
		}

		ExitTile* newTile = new ExitTile(col * tileSize, row * tileSize, tileSize, selectedTileType, false, false, nullptr, false, 4);
		replaceTile(row, col, newTile);
		exit = newTile;
		break;
	}
	case PLAYER:
	{
		int typeAtClickedLocation = getTileAtMapIndex(row, col)->getType();
		if (getTileAtMapIndex(row, col)->isSolid() || typeAtClickedLocation == PIT || typeAtClickedLocation == DOOR || typeAtClickedLocation == LADDER)
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
	case ENEMY:
	{
		int typeAtClickedLocation = getTileAtMapIndex(row, col)->getType();
		if (getTileAtMapIndex(row, col)->isSolid() || typeAtClickedLocation == PIT || typeAtClickedLocation == DOOR || typeAtClickedLocation == LADDER)
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
	case ARROW:
	{
		int typeAtClickedLocation = getTileAtMapIndex(row, col)->getType();
		if (getTileAtMapIndex(row, col)->isSolid() || typeAtClickedLocation == DOOR || typeAtClickedLocation == LADDER)
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
	int testMap[20][30] = { { WALL_TOP_LEFT_CORNER, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_TOP_RIGHT_CORNER },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_VERTICAL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL_VERTICAL },
							{ WALL_BOTTOM_LEFT_CORNER, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_BOTTOM_RIGHT_CORNER } };
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
	if (temp == exit)
	{
		exit == nullptr;
	}
	tileMap.at((row*mapCols) + col) = newTile;
	delete temp;
}

void EditorModel::removeExit()
{
	int row = exit->getTileSpace()->y / tileSize;
	int col = exit->getTileSpace()->x / tileSize;
	//Replace old exit tile with a generic floor tile if it was a ladder, or a wall if it was a door
	if (exit->getType() == LADDER)
	{
		Tile* newTile = new Tile(row * tileSize, col * tileSize, tileSize, FLOOR, false, false, nullptr);
		replaceTile(row * tileSize, col * tileSize, newTile);
	}
	else //DOOR
	{
		placeWall(row, col);
	}
}

void EditorModel::placeWall(int row, int col)
{

}

void EditorModel::selectWallType(int row, int col)
{

}
