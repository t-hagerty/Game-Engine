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
		if (!isAWall(getTileAtMapIndex(row, col)->getType()))
		{
			break;
		}
		short direction = determineDoorDirection(row, col);
		if (direction == -1)
		{
			break;
		}
		if (exit != nullptr)
		{
			removeExit();
		}
		ExitTile* newTile = new ExitTile(col * tileSize, row * tileSize, tileSize, selectedTileType, false, false, nullptr, false, direction);
		replaceTile(row, col, newTile);
		updateSurroundingWalls(row, col);
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
	case SWITCH:
	{
		bool wasWall = ((isAWall(getTileAtMapIndex(row, col)->getType())) ? true : false);
		replaceTile(row, col, new Switch(col * tileSize, row * tileSize, tileSize));
		if (wasWall)
		{
			updateSurroundingWalls(row, col);
		}
		break;
	}
	case SWITCH_WEIGHTED:
	{
		break;
	}
	case SWITCH_LEVER:
	{
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
	case WALL_SINGLE: //Will be the type passed by the generic "place a wall" button and the actual type will be calculated.
		placeWall(row, col);
		break;
	default:
	{
		bool wasWall = ((isAWall(getTileAtMapIndex(row, col)->getType())) ? true : false);
		Tile* newTile = new Tile(col * tileSize, row * tileSize, tileSize, selectedTileType, setIsSolid(selectedTileType), ((selectedTileType == PIT) ? true : false), nullptr);
		replaceTile(row, col, newTile);
		if (wasWall)
		{
			updateSurroundingWalls(row, col);
		}
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
	int testMap[20][30] = { { WALL_TOP_LEFT_CORNER, WALL_HORIZONTAL, DOOR, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_HORIZONTAL, WALL_TOP_RIGHT_CORNER },
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
			if (testMap[r][c] == DOOR || testMap[r][c] == LADDER)
			{
				exit = new ExitTile(c * tileSize, r * tileSize, tileSize, testMap[r][c], false, false, nullptr, false, 0);
				tileMap.push_back(exit);
			}
			else
			{
				tileMap.push_back(new Tile(c * tileSize, r * tileSize, tileSize, testMap[r][c], setIsSolid(testMap[r][c]), ((testMap[r][c] == PIT) ? true : false), nullptr));
			}
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
		exit = nullptr;
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
		Tile* newTile = new Tile(col * tileSize, row * tileSize, tileSize, FLOOR, false, false, nullptr);
		replaceTile(row, col, newTile);
	}
	else //DOOR
	{
		placeWall(row, col);
	}
}

int EditorModel::determineDoorDirection(int row, int col)
{
	int direction = -1;
	short wallsum = 0;
	if (row == 0 || isAWall(getTileAtMapIndex(row - 1, col)->getType()))
	{
		wallsum += 1;
	}
	if (col == mapCols - 1 || isAWall(getTileAtMapIndex(row, col + 1)->getType()))
	{
		wallsum += 2;
	}
	if (row == mapRows - 1 || isAWall(getTileAtMapIndex(row + 1, col)->getType()))
	{
		wallsum += 4;
	}
	if (col == 0 || isAWall(getTileAtMapIndex(row, col - 1)->getType()))
	{
		wallsum += 8;
	}
	switch (wallsum)
	{
	case 11:
		direction = 0;
		break;
	case 7:
		direction = 1;
		break;
	case 14:
		direction = 2;
		break;
	case 13:
		direction = 3;
		break;
	default:
		break;
	}
	return direction;
}

void EditorModel::placeWall(int row, int col)
{
	selectWallType(row, col);
	updateSurroundingWalls(row, col);
}

void EditorModel::updateSurroundingWalls(int row, int col)
{
	if (row != 0 && isAWall(getTileAtMapIndex(row - 1, col)->getType()))
	{
		selectWallType(row - 1, col);
	}
	if (col != 0 && isAWall(getTileAtMapIndex(row, col - 1)->getType()))
	{
		selectWallType(row, col - 1);
	}
	if (row != mapRows - 1 && isAWall(getTileAtMapIndex(row + 1, col)->getType()))
	{
		selectWallType(row + 1, col);
	}
	if (col != mapCols - 1 && isAWall(getTileAtMapIndex(row, col + 1)->getType()))
	{
		selectWallType(row, col + 1);
	}
}

void EditorModel::selectWallType(int row, int col)
{
	short wallsum = 0;
	if (row == 0 || isAWall(getTileAtMapIndex(row - 1, col)->getType()))
	{
		wallsum += 1;
	}
	if (col == mapCols -1 || isAWall(getTileAtMapIndex(row, col + 1)->getType()))
	{
		wallsum += 2;
	}
	if (row == mapRows -1 || isAWall(getTileAtMapIndex(row + 1, col)->getType()))
	{
		wallsum += 4;
	}
	if (col == 0 || isAWall(getTileAtMapIndex(row, col - 1)->getType()))
	{
		wallsum += 8;
	}
	replaceTile(row, col, new Tile(col * tileSize, row * tileSize, tileSize, wallsum, true, false, nullptr));
}

bool EditorModel::isAWall(int type)
{
	switch (type)
	{
	case WALL_SINGLE:
		return true;
	case WALL_BOTTOM_END:
		return true;
	case WALL_LEFT_END:
		return true;
	case WALL_BOTTOM_LEFT_CORNER:
		return true;
	case WALL_TOP_END:
		return true;
	case WALL_VERTICAL:
		return true;
	case WALL_TOP_LEFT_CORNER:
		return true;
	case WALL_VERTICAL_LEFT:
		return true;
	case WALL_RIGHT_END:
		return true;
	case WALL_BOTTOM_RIGHT_CORNER:
		return true;
	case WALL_HORIZONTAL:
		return true;
	case WALL_HORIZONTAL_BOTTOM:
		return true;
	case WALL_TOP_RIGHT_CORNER:
		return true;
	case WALL_VERTICAL_RIGHT:
		return true;
	case WALL_HORIZONTAL_TOP:
		return true;
	case WALL_FILLED:
		return true;
	default:
		return false;
	}
}
