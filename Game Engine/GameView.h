#pragma once
#include <SDL.h>

class GameView
{
public:
	GameView();
	~GameView();
	int getWindowWidth() const;
	int getWindowHeight() const;
	void renderClear(int red = 0xFF, int green = 0xFF, int blue = 0xFF, int alpha = 0xFF) const;
	void renderUpdate() const;
	void renderRectangle(double posX, double posY, int width, int height) const; //FOR TESTING ONLY
	void close();
private:
	SDL_Window* gameWindow = nullptr;
	SDL_Surface* gScreenSurface = nullptr;
	SDL_Renderer* gameRenderer = nullptr;
	const int WINDOW_HEIGHT = 400;
	const int WINDOW_WIDTH = 600;
	bool init();
};

