#include "GameView_LevelTest.h"



GameView_LevelTest::GameView_LevelTest(int levelWidth, int levelHeight, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer)
	: GameView(levelWidth, levelHeight, windowW, windowH, window, screen, renderer)
{
}


GameView_LevelTest::~GameView_LevelTest()
{
}

void GameView_LevelTest::setButtonHandlers(EventHandler retryButtonHandler, EventHandler editorButtonHandler, EventHandler mainMenuButtonHandler)
{
	//Nothing is actually different right now, but it very possibly will be in the future
	retryButton = addButton((windowWidth / 2) - 200, (windowHeight / 2) - 75, 400, 150, false, "default_button.bmp", "RETRY?", retryButtonHandler);
	restartButton->setEventHandler(retryButtonHandler);
	editorButton->setEventHandler(editorButtonHandler);
	mainMenuButton->setEventHandler(mainMenuButtonHandler);
}

bool GameView_LevelTest::initGUI()
{
	//Nothing is actually different right now, but it very possibly will be in the future
	bool success = true;
	double width = 50;
	if (50 < windowWidth / 15)
	{
		width = windowWidth / 15;
	}
	double height = 20;
	if (20 < windowHeight / 15)
	{
		height = windowHeight / 15;
	}
	double posX = windowWidth - width;
	double posY = 0;
	menuButton = new Button(posX, posY, width, height, true, "default_button.bmp", gScreenSurface, gameRenderer, "MENU", std::bind(&GameView::toggleMenu, this));
	addButton(menuButton);
	menu = new ButtonMenu(225, 100, 150, 200, false, "menu.bmp", gScreenSurface, gameRenderer, 150, 10, 8, 1);
	gui.push_back(menu);
	restartButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "RESTART", nullptr);
	addButton(restartButton);
	menu->addButton(restartButton);
	settingsButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "SETTINGS", std::bind(&GameView::settingsButtonPressed, this));
	addButton(settingsButton);
	menu->addButton(settingsButton);
	editorButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "BACK TO EDITOR", nullptr);
	addButton(editorButton);
	menu->addButton(editorButton);
	mainMenuButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "MAIN MENU", nullptr);
	addButton(mainMenuButton);
	menu->addButton(mainMenuButton);
	return success;
}
