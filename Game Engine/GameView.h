#pragma once
#include <SDL.h>
#undef main
#include <string>
#include <vector>
#include "Tile.h"
#include "Entity.h"

class GameView
{
public:
	GameView(int levelWidth, int levelHeight);
	~GameView();
	int getWindowWidth() const;
	int getWindowHeight() const;
	void renderClear(int red = 0xFF, int green = 0xFF, int blue = 0xFF, int alpha = 0xFF) const;
	void renderUpdate() const;
	void renderTileMap(std::vector<Tile*> map, int rows, int cols, int tileSize);
	void renderRectangle(double posX, double posY, int width, int height) const; //FOR TESTING ONLY
	void renderEntitySprite(Entity* e, int frame);
	void positionCamera(SDL_Rect* playerBox) const;
	void close();
private:
	SDL_Window* gameWindow = nullptr;
	SDL_Surface* gScreenSurface = nullptr;
	SDL_Renderer* gameRenderer = nullptr;
	SDL_Rect* camera = new SDL_Rect();
	const int WINDOW_HEIGHT = 400;
	const int WINDOW_WIDTH = 600;
	int levelWidth;
	int levelHeight;
	std::vector<SDL_Texture*> tileSet;
	bool init();
	SDL_Texture* loadTexture(std::string filePath);
	SDL_Surface* loadImage(std::string filePath) const;
	enum textures
	{
		FLOOR,
		WALL_BOTTOM_LEFT_CORNER,
		WALL_BOTTOM_RIGHT_CORNER,
		WALL_TOP_LEFT_CORNER,
		WALL_TOP_RIGHT_CORNER,
		WALL_HORIZONTAL,
		WALL_VERTICAL,
		GRASS,
		BARRIER
	};

};

