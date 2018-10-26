#pragma once
#include <SDL.h>
#undef main
#include <string>
#include <vector>
#include "Tile.h"
#include "Entity.h"
#include "Button.h"
#include "Image.h"

class GameView
{
public:
	typedef std::function<void()> EventHandler;
	GameView(int levelWidth, int levelHeight);
	~GameView();
	void setWindowWidth(int newWidth);
	int getWindowWidth() const;
	void setWindowHeight(int newHeight);
	int getWindowHeight() const;
	void setZoomScale(float newScale);
	float getZoomScale();
	void setIsPaused(bool paused);
	bool getIsPaused();
	void setRetryButton(EventHandler buttonEventHandler);
	std::vector<Button*> getButtons();
	void renderClear(int red = 0xFF, int green = 0xFF, int blue = 0xFF, int alpha = 0xFF) const;
	void renderUpdate() const;
	void renderTileMap(std::vector<Tile*> map, int rows, int cols, int tileSize, int frame);
	void renderRectangle(double posX, double posY, int width, int height) const; //FOR TESTING ONLY
	void renderEntitySprite(Entity* e, int frame);
	void renderText(std::string text, SDL_Rect* textRect);
	void renderPlayerInfo(double playerHealth);
	void renderButtons();
	void renderAButton(Button* aButton);
	void renderImage(Image* anImage);
	void renderGUIElements();
	void positionCamera(SDL_Rect* playerBox) const;
	void addButton(Button* aButton);
	Button* addButton(double posX, double posY, double width, double height, bool isVisible, std::string filePath, std::string buttonText, EventHandler eventHandler);
	void toggleMenu();
	void isGameOverScreen(bool isGameOver);
	bool getIsGameOverScreen();
	void close();
private:
	SDL_Window* gameWindow = nullptr;
	SDL_Surface* gScreenSurface = nullptr;
	SDL_Renderer* gameRenderer = nullptr;
	SDL_Rect* camera = new SDL_Rect();
	int windowHeight = 400;
	int windowWidth = 600;
	float zoomScale = 1;
	int levelWidth;
	int levelHeight;
	bool isPaused = false;
	std::vector<SDL_Texture*> tileSet;
	bool init();
	bool initGUI();
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
		BARRIER,
		DOWN_TREADMILL,
		RIGHT_TREADMILL,
		LEFT_TREADMILL,
		UP_TREADMILL,
		ICE,
		MUD,
		PIT,
		LAVA,
		SPIKES
	};
	
	std::vector<Button*> buttons;
	Button* menuButton;
	Button* retryButton;
	Image* menuBackground;
};

