#pragma once
#include <SDL.h>
#include <string>
#include <vector>

class GameView
{
public:
	GameView();
	~GameView();
	int getWindowWidth() const;
	int getWindowHeight() const;
	void renderClear(int red = 0xFF, int green = 0xFF, int blue = 0xFF, int alpha = 0xFF) const;
	void renderUpdate() const;
	void renderTileMap(int** map, int rows, int cols, int tileSize);
	void renderRectangle(double posX, double posY, int width, int height) const; //FOR TESTING ONLY
	void close();
private:
	SDL_Window* gameWindow = nullptr;
	SDL_Surface* gScreenSurface = nullptr;
	SDL_Renderer* gameRenderer = nullptr;
	const int WINDOW_HEIGHT = 400;
	const int WINDOW_WIDTH = 600;
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

