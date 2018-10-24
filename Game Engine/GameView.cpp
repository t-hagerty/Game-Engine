#include "GameView.h"
#include <cstdio>
#include <SDL_ttf.h>
#include <sstream>
#include <functional>

/*
 * Game rendering using SDL was learned from http://lazyfoo.net/tutorials/SDL/index.php and some of the code here is currently borrowed from lessons at said site.
 */

GameView::GameView(int levelW, int levelH)
{
	if(!init())
	{
		printf("Failed to initialize\n");
	}
	else
	{
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
	}
	camera->h = windowHeight;
	camera->w = windowWidth;
	levelWidth = levelW;
	levelHeight = levelH;
	initGUI();
}


GameView::~GameView()
{
	renderClear(0xFF, 0xFF, 0xFF, 0xFF);
	close();
}

void GameView::setWindowWidth(int newWidth)
{
	if (windowWidth != newWidth)
	{
		if (zoomScale != 1)
		{
			camera->w += ((newWidth - windowWidth) / zoomScale);
			//printf("New width: %d, new camera width: %d \n", newWidth, camera->w);
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
}

bool GameView::getIsPaused()
{
	return isPaused;
}

void GameView::setRetryButton(EventHandler buttonEventHandler)
{
	retryButton = addButton((windowWidth / 2) - 200, (windowHeight / 2) - 75, 400, 150, false, "default_button.bmp", "RETRY?", buttonEventHandler);
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

void GameView::renderTileMap(std::vector<Tile*> map, int rows, int cols, int tileSize)
{
	for (Tile* t : map)
	{
		if (SDL_HasIntersection(t->getTileSpace(), camera))
		{
			//Set rendering space and render to screen
			SDL_Rect renderQuad = { (t->getTileSpace()->x - camera->x) * zoomScale , (t->getTileSpace()->y - camera->y) * zoomScale , t->getTileSpace()->w * zoomScale, t->getTileSpace()->h * zoomScale };

			//Render to screen
			SDL_RenderCopyEx(gameRenderer, tileSet.at(t->getType()), NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
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
		printf("Health text could not display. TTF Error: %s\n", TTF_GetError());
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
	std::string s;
	s = std::to_string((int)playerHealth);
	SDL_Rect messageRect;
	messageRect.x = 20;
	messageRect.y = 0;
	messageRect.w = 48;
	messageRect.h = 48;

	renderText(s, &messageRect);
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
	renderButtons();
	renderImage(menuBackground);
}

void GameView::positionCamera(SDL_Rect * playerBox) const
{
	//Center the camera over the player
	camera->x = ((playerBox->x ) + (playerBox->w ) / 2) - (windowWidth / zoomScale ) / 2;
	camera->y = ((playerBox->y ) + (playerBox->h ) / 2) - (windowHeight / zoomScale ) / 2;
	//printf("Camera x: %d Camera y: %d Camera width: %d Camera height: %d \n", camera->x, camera->y, camera->w, camera->h);
	//printf("player x: %d player y: %d player width: %d player height: %d \n", playerBox->x, playerBox->y, playerBox->w, playerBox->h);

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
	menuBackground->toggleVisibility();
	renderUpdate();
}

void GameView::isGameOverScreen(bool isGameOver)
{
	setIsPaused(isGameOver);
	retryButton->setIsVisible(isGameOver);
}

bool GameView::getIsGameOverScreen()
{
	return retryButton->getIsVisible();
}

bool GameView::init()
{
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled! \n");
		}

		//Create window
		gameWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
		if (gameWindow == nullptr)
		{
			printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//SDL_SetWindowResizable(gameWindow, SDL_TRUE);
			//Create renderer for window
			gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gameRenderer == nullptr)
			{
				printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gameWindow);
		}
	}
	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	return success;
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
	menuBackground = new Image(225, 100, 150, 200, false, "menu.bmp", gScreenSurface, gameRenderer);

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

void GameView::close()
{	
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameWindow = nullptr;
	gameRenderer = nullptr;
	TTF_Quit();
	SDL_Quit();
}
