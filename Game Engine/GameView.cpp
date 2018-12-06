#include "GameView.h"
#include <cstdio>
#include <SDL_ttf.h>
#include <sstream>
#include <functional>

/*
 * Game rendering using SDL was learned from http://lazyfoo.net/tutorials/SDL/index.php and some of the code here is currently borrowed from lessons at said site.
 */

GameView::GameView(int levelW, int levelH, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer)
{
	gameWindow = window;
	gScreenSurface = screen;
	gameRenderer = renderer;

	tileSet.insert(tileSet.end(), loadTexture("map_tiles/floor.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_bottom_left_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_bottom_right_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_top_left_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_top_right_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_horizontal.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_vertical.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/grass.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/barrier.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/down_treadmill.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/right_treadmill.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/left_treadmill.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/up_treadmill.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/ice.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/mud.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/pit.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/lava.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/spikes.bmp"));
	SDL_UpdateWindowSurface(gameWindow);

	windowWidth = windowW;
	windowHeight = windowH;
	camera->h = windowHeight;
	camera->w = windowWidth;
	levelWidth = levelW;
	levelHeight = levelH;
	initGUI();
}


GameView::~GameView()
{
	renderClear(0xFF, 0xFF, 0xFF, 0xFF);
	delete camera;
	for (GUIElement* g : gui)
	{
		delete g;
	}
	for (SDL_Texture* t : tileSet)
	{
		SDL_DestroyTexture(t);
	}
}

void GameView::setWindowWidth(int newWidth)
{
	if (windowWidth != newWidth)
	{
		if (zoomScale != 1)
		{
			camera->w += ((newWidth - windowWidth) / zoomScale);
		}
		else
		{
			camera->w = newWidth;
		}
		windowWidth = newWidth;
	}
}

int GameView::getWindowWidth() const
{
	return windowWidth;
}

void GameView::setWindowHeight(int newHeight)
{
	if (windowHeight != newHeight)
	{
		if (zoomScale != 1)
		{
			camera->h += ((newHeight - windowHeight) / zoomScale);
			//printf("New height: %d, new camera height: %d \n", newHeight, camera->h);
		}
		else
		{
			camera->h = newHeight;
		}
		windowHeight = newHeight;
	}
}

int GameView::getWindowHeight() const
{
	return windowHeight;
}

void GameView::setZoomScale(float newScale)
{
	zoomScale = newScale;
	camera->w = camera->w / zoomScale;
	camera->h = camera->h / zoomScale;
}

float GameView::getZoomScale()
{
	return zoomScale;
}

void GameView::setIsPaused(bool paused)
{
	isPaused = paused;
	if (!isPaused)
	{
		menu->setIsVisible(paused);
	}
}

bool GameView::getIsPaused()
{
	return isPaused;
}

void GameView::setButtonHandlers(EventHandler retryButtonHandler, EventHandler editorButtonHandler, EventHandler mainMenuButtonHandler)
{
	retryButton = addButton((windowWidth / 2) - 200, (windowHeight / 2) - 75, 400, 150, false, "default_button.bmp", "RETRY?", retryButtonHandler);
	restartButton->setEventHandler(retryButtonHandler);
	editorButton->setEventHandler(editorButtonHandler);
	mainMenuButton->setEventHandler(mainMenuButtonHandler);
}

std::vector<Button*> GameView::getButtons()
{
	return buttons;
}

void GameView::renderClear(int red, int green, int blue, int alpha) const
{
	//Clear screen
	SDL_SetRenderDrawColor(gameRenderer, red, green, blue, alpha);
	SDL_RenderClear(gameRenderer);
}

void GameView::renderUpdate() const
{
	//Update screen
	SDL_RenderPresent(gameRenderer);
}

void GameView::renderTileMap(std::vector<Tile*> map, int rows, int cols, int tileSize, int frame)
{
	for (Tile* t : map)
	{
		if (SDL_HasIntersection(t->getTileSpace(), camera))
		{
			//Set rendering space and render to screen
			SDL_Rect renderQuad = { (t->getTileSpace()->x - camera->x) * zoomScale , (t->getTileSpace()->y - camera->y) * zoomScale , t->getTileSpace()->w * zoomScale, t->getTileSpace()->h * zoomScale };
			SDL_Rect textureFrameClip = { 0, t->getAnimationFrame() * t->getTextureFrameHeight(), t->getTextureFrameWidth(), t->getTextureFrameHeight() };
			//Render to screen
			SDL_RenderCopyEx(gameRenderer, tileSet.at(t->getType()), &textureFrameClip, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
		}
		if ((frame == 0 || frame == 10 || frame == 20 || frame == 30 || frame == 40 || frame == 50) && !isPaused)
		{
			t->incrementAnimationFrame();
		}
	}
}

void GameView::renderRectangle(double posX, double posY, int width, int height) const
{
	SDL_Rect fillRect = { posX , posY , width , height  };
	fillRect.x -= camera->x ;
	fillRect.y -= camera->y ;
	SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderFillRect(gameRenderer, &fillRect);
}

void GameView::renderEntitySprite(Entity* e, int frame)
{
	if (e->getSpriteSheet() == nullptr)
	{
		e->setSpriteSheet(loadTexture(e->getSpriteFilePath()));
	}
	if(SDL_HasIntersection(e->getCollisionBox(), camera))
	{
	SDL_Rect spriteSheetClip = { e->getAnimationFrame() * e->getSpriteWidth(),
								e->getSpriteDirection() * e->getSpriteHeight(),
								e->getSpriteWidth(), e->getSpriteHeight() };
	SDL_Rect fillRect = { (e->getPosX() - camera->x) * zoomScale, (e->getPosY() - camera->y) * zoomScale , e->getWidth() * zoomScale , e->getHeight() * zoomScale };
	SDL_RenderCopy(gameRenderer, e->getSpriteSheet(), &spriteSheetClip, &fillRect);
	}
	if((e->getVelocityX() != 0 || e->getVelocityY() != 0))
	{
		if (e->getKnockbackTimer() == 0 && (frame == 0 || frame == 15 || frame == 30 || frame == 45) && !isPaused)
		{
			e->incrementAnimationFrame();
		}
	}
	else
	{
		e->setAnimationFrame(0);
	}
}

void GameView::renderText(std::string text, SDL_Rect * textRect)
{
	if (text == "")
	{
		return;
	}
	std::stringstream s;
	s << text;

	TTF_Font* font = TTF_OpenFont("segoeui.ttf", 24);
	if (!font)
	{
		printf("TTF_OpenFont Error: %s\n", TTF_GetError());
	}
	SDL_Color textColor = { 0, 0, 0 };
	SDL_Surface* messageSurface;
	if (!(messageSurface = TTF_RenderText_Solid(font, s.str().c_str(), textColor)))
	{
		printf("Text could not display. TTF Error: %s\n", TTF_GetError());
	}
	else
	{
		SDL_Texture* message = SDL_CreateTextureFromSurface(gameRenderer, messageSurface);

		SDL_RenderCopy(gameRenderer, message, NULL, textRect);
		SDL_DestroyTexture(message);
	}
	SDL_FreeSurface(messageSurface);
	TTF_CloseFont(font);
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

void GameView::renderButtons()
{
	for (Button* aButton : buttons)
	{
		if (aButton->getIsVisible())
		{
			renderAButton(aButton);
		}
	}
}

void GameView::renderAButton(Button * aButton)
{
	if (aButton->getIsVisible())
	{
		SDL_Rect spriteSheetClip = { aButton->getButtonState() * aButton->getButtonImageWidth(),
			0,
			aButton->getButtonImageWidth(), aButton->getButtonImageHeight() };
		SDL_Rect renderRect = { aButton->getRect()->x , aButton->getRect()->y , aButton->getRect()->w , aButton->getRect()->h  };
		SDL_RenderCopy(gameRenderer, aButton->getTexture(), &spriteSheetClip, &renderRect);
		renderText(aButton->getButtonText(), aButton->getRect());
	}
}

void GameView::renderImage(Image * anImage)
{
	if (!anImage->getIsVisible())
	{
		return;
	}
	else
	{
		SDL_Rect renderRect = { anImage->getRect()->x , anImage->getRect()->y , anImage->getRect()->w , anImage->getRect()->h  };
		SDL_RenderCopy(gameRenderer, anImage->getTexture(), NULL, &renderRect);
	}
}

void GameView::renderGUIElements()
{
	for (GUIElement* g : gui)
	{
		g->render();
	}
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

void GameView::addButton(Button * aButton)
{
	buttons.push_back(aButton);
	gui.push_back(aButton);
}

Button* GameView::addButton(double posX, double posY, double width, double height, bool isVisible, std::string filePath, std::string buttonText, EventHandler eventHandler)
{
	Button* aButton = new Button(posX, posY, width, height, isVisible, filePath, gScreenSurface, gameRenderer, buttonText, eventHandler);
	addButton(aButton);
	return aButton;
}

void GameView::toggleMenu()
{
	isPaused = !isPaused;
	menu->toggleVisibility();
	renderUpdate();
}

void GameView::settingsButtonPressed()
{
}

void GameView::isGameOverScreen(bool isGameOver)
{
	setIsPaused(isGameOver);
	retryButton->setIsVisible(isGameOver);
	if (!isGameOver)
	{
		menu->setIsVisible(false);
	}
}

bool GameView::getIsGameOverScreen()
{
	return retryButton->getIsVisible();
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
	return success;
}

SDL_Texture* GameView::loadTexture(std::string filePath)
{
	SDL_Texture* texture = nullptr;

	SDL_Surface* textureImage = loadImage(filePath);
	if(textureImage == nullptr)
	{
		printf("Unable to load image %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
	}
	else
	{
		SDL_SetColorKey(textureImage, SDL_TRUE, SDL_MapRGB(textureImage->format, 0xFF, 0, 0xFF));
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		texture = SDL_CreateTextureFromSurface(gameRenderer, textureImage);
		if(texture == nullptr)
		{
			printf("Unable to create texture from %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(textureImage);
	}

	return texture;
}

SDL_Surface * GameView::loadImage(std::string filePath) const
{
	SDL_Surface* optimizedImage = nullptr;

	SDL_Surface* loadedImage = SDL_LoadBMP(filePath.c_str());
	if(loadedImage == nullptr)
	{
		printf("Unable to load image %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
	}
	else
	{
		optimizedImage = SDL_ConvertSurface(loadedImage, gScreenSurface->format, 0); //convert to image on surface to pixel format of the game's screen surface for optimization
		if (optimizedImage == nullptr)
		{
			printf("Unable to optimize image %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedImage); //free memory of loadedImage
	}
	return optimizedImage;
}
