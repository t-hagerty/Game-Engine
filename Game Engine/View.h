#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "Tile.h"
#include "Entity.h"
#include "Button.h"
#include "Image.h"
#include "ButtonMenu.h"
#include "HealthBar.h"
class View
{
public:
	typedef std::function<void(int)> EventHandler;
	View(int levelW, int levelH, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer);
	~View();
	int getLevelWidth();
	int getLevelHeight();
	void setWindowWidth(int newWidth);
	int getWindowWidth() const;
	void setWindowHeight(int newHeight);
	int getWindowHeight() const;
	SDL_Window* getWindow();
	SDL_Surface* getSurface();
	SDL_Renderer* getRenderer();
	void setZoomScale(float newScale);
	float getZoomScale();
	void setIsPaused(bool paused);
	bool getIsPaused();
	void renderClear(int red = 0xFF, int green = 0xFF, int blue = 0xFF, int alpha = 0xFF) const;
	void renderUpdate() const;
	virtual void renderTileMap(std::vector<Tile*> map, int rows, int cols, int tileSize, int frame);
	void renderRectangle(double posX, double posY, int width, int height) const; //FOR TESTING ONLY
	virtual void renderEntitySprite(Entity* e, int frame);
	SDL_Texture* createTextTexture(std::string text);
	void renderText(std::string text, SDL_Rect* textRect);
	void renderButtons();
	void renderAButton(Button* aButton);
	void renderImage(Image* anImage);
	void renderGUIElements();
	std::vector<Button*> getButtons();
	void addButton(Button* aButton);
	Button* addButton(double posX, double posY, double width, double height, bool isVisible, std::string filePath, std::string buttonText, EventHandler eventHandler);
	void toggleMenu();
protected:
	SDL_Window* gameWindow = nullptr;
	SDL_Surface* gScreenSurface = nullptr;
	SDL_Renderer* gameRenderer = nullptr;
	SDL_Rect* camera = new SDL_Rect();
	int windowHeight;
	int windowWidth;
	float zoomScale = 1;
	int levelWidth;
	int levelHeight;
	bool isPaused = false;
	std::vector<SDL_Texture*> tileSet;

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
		SPIKES,
		DOOR,
		LADDER,
		WALL_BOTTOM_END,
		WALL_LEFT_END,
		WALL_RIGHT_END,
		WALL_TOP_END,
		WALL_FILLED,
		WALL_HORIZONTAL_BOTTOM,
		WALL_HORIZONTAL_TOP,
		WALL_VERTICAL_LEFT,
		WALL_VERTICAL_RIGHT,
		WALL_SINGLE
	};
	std::vector<GUIElement*> gui;
	std::vector<Button*> buttons;
	Button* menuButton;
	Button* mainMenuButton;
	Button* settingsButton;
	ButtonMenu* menu;

	virtual bool initGUI() = 0;
	SDL_Texture* loadTexture(std::string filePath);
	SDL_Surface* loadImage(std::string filePath) const;
};

