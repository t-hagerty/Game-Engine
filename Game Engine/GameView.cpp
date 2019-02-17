#include "GameView.h"
#include <cstdio>
#include <SDL_ttf.h>
#include <sstream>
#include <functional>

/*
 * Game rendering using SDL was learned from http://lazyfoo.net/tutorials/SDL/index.php and some of the code here is currently borrowed from lessons at said site.
 */

GameView::GameView(int levelW, int levelH, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer) :
	View(levelW, levelH, windowW, windowH, window, screen, renderer)
{
	initGUI();
}


GameView::~GameView()
{
}

void GameView::setButtonHandlers(EventHandler retryButtonHandler, EventHandler editorButtonHandler, EventHandler mainMenuButtonHandler)
{
	retryButton = addButton((windowWidth / 2) - 200, (windowHeight / 2) - 75, 400, 150, false, "default_button.bmp", "RETRY?", retryButtonHandler);
	restartButton->setEventHandler(retryButtonHandler);
	editorButton->setEventHandler(editorButtonHandler);
	mainMenuButton->setEventHandler(mainMenuButtonHandler);
}

void GameView::renderPlayerInfo(double playerHealth)
{
	if (hpBar == nullptr)
	{
		hpBar = new HealthBar(0, 0, 160, 32, true, "menu.bmp", gScreenSurface, gameRenderer, playerHealth);
		gui.push_back(hpBar);
	}
	hpBar->update(playerHealth);
	hpBar->render();
}

void GameView::positionCamera(SDL_Rect * playerBox) const
{
	//Center the camera over the player
	camera->x = ((playerBox->x ) + (playerBox->w ) / 2) - (windowWidth / zoomScale ) / 2;
	camera->y = ((playerBox->y ) + (playerBox->h ) / 2) - (windowHeight / zoomScale ) / 2;

	//Keep the camera in bounds
	if (camera->x < 0)
	{
		camera->x = 0;
	}
	else if (camera->x > levelWidth  - camera->w)
	{
		camera->x = levelWidth  - camera->w;
	}
	if (camera->y < 0)
	{
		camera->y = 0;
	}
	else if (camera->y > levelHeight  - camera->h)
	{
		camera->y = levelHeight  - camera->h;
	}
}

void GameView::settingsButtonPressed()
{
}

void GameView::isGameOverScreen(bool isGameOver)
{
	setIsPaused(isGameOver);
	retryButton->setIsVisible(isGameOver);
	menuButton->setIsVisible(!isGameOver);
	if (!isGameOver)
	{
		menu->setIsVisible(false);
	}
}

bool GameView::getIsGameOverScreen()
{
	return retryButton->getIsVisible();
}

void GameView::isLevelWonScreen(bool isLevelWon)
{
	setIsPaused(isLevelWon);
	retryButton->setIsVisible(isLevelWon);
	levelWinMessage->setIsVisible(isLevelWon);
	menuButton->setIsVisible(!isLevelWon);
	if (!isLevelWon)
	{
		menu->setIsVisible(false);
	}
}

bool GameView::getIsLevelWonScreen()
{
	return levelWinMessage->getIsVisible();
}

bool GameView::initGUI()
{
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
	editorButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "EDITOR", nullptr);
	addButton(editorButton);
	menu->addButton(editorButton);
	mainMenuButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "MAIN MENU", nullptr);
	addButton(mainMenuButton);
	menu->addButton(mainMenuButton);
	levelWinMessage = new Image((windowWidth / 2) - 200, 25, 400, 100, false, createTextTexture("Winner!"), 1, gScreenSurface, gameRenderer);
	gui.push_back(levelWinMessage);
	return success;
}
