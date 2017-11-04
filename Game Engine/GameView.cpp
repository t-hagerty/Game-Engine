#include "GameView.h"
#include <cstdio>
#include <SDL_ttf.h>
#include <sstream>

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
		SDL_UpdateWindowSurface(gameWindow);
	}
	camera->x = 0;
	camera->y = 0;
	camera->h = WINDOW_HEIGHT;
	camera->w = WINDOW_WIDTH;
	levelWidth = levelW;
	levelHeight = levelH;
}


GameView::~GameView()
{
	//Clear screen
	SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gameRenderer);
	close();
}

int GameView::getWindowWidth() const
{
	return WINDOW_WIDTH;
}

int GameView::getWindowHeight() const
{
	return WINDOW_HEIGHT;
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
		if(SDL_HasIntersection(&t->getTileSpace(), camera))
		{
			//Set rendering space and render to screen
			SDL_Rect renderQuad = { t->getTileSpace().x - camera->x, t->getTileSpace().y - camera->y, t->getTileSpace().w, t->getTileSpace().h };

			//Render to screen
			SDL_RenderCopyEx(gameRenderer, tileSet.at(t->getType()), NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
		}
	}
}

void GameView::renderRectangle(double posX, double posY, int width, int height) const
{
	SDL_Rect fillRect = { posX, posY, width, height};
	fillRect.x -= camera->x;
	fillRect.y -= camera->y;
	SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderFillRect(gameRenderer, &fillRect);
}

void GameView::renderEntitySprite(Entity* e, int frame)
{
	if(e->getSpriteSheet() == nullptr)
	{
		e->setSpriteSheet(loadTexture(e->getSpriteFilePath()));
	}
	SDL_Rect spriteSheetClip = { e->getAnimationFrame() * e->getSpriteWidth(), 
								e->getSpriteDirection() * e->getSpriteHeight(), 
								e->getSpriteWidth(), e->getSpriteHeight() };
	SDL_Rect fillRect = { e->getPosX(), e->getPosY(), e->getWidth(), e->getHeight() };
	fillRect.x -= camera->x;
	fillRect.y -= camera->y;
	SDL_RenderCopy(gameRenderer, e->getSpriteSheet(), &spriteSheetClip, &fillRect);
	if((e->getVelocityX() != 0 || e->getVelocityY() != 0))
	{
		if (frame == 0 || frame == 15 || frame == 30 || frame == 45)
		{
			e->incrementAnimationFrame();
		}
	}
	else
	{
		e->setAnimationFrame(0);
	}
}

void GameView::renderPlayerInfo(double playerHealth)
{
	std::stringstream s;
	s << playerHealth;

	TTF_Font* font = TTF_OpenFont("segoeui.ttf", 24);
	SDL_Color textColor = {0, 0, 0};
	SDL_Surface* messageSurface = TTF_RenderText_Solid(font, s.str().c_str(), textColor);
	SDL_Texture* message = SDL_CreateTextureFromSurface(gameRenderer, messageSurface);

	SDL_Rect messageRect;
	messageRect.x = 0;
	messageRect.y = 0;
	messageRect.w = 100;
	messageRect.h = 100;
	SDL_RenderCopy(gameRenderer, message, NULL, &messageRect);
	SDL_FreeSurface(messageSurface);
}

void GameView::positionCamera(SDL_Rect * playerBox) const
{
	//Center the camera over the dot
	camera->x = (playerBox->x + playerBox->w / 2) - WINDOW_WIDTH / 2;
	camera->y = (playerBox->y + playerBox->h / 2) - WINDOW_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera->x < 0)
	{
		camera->x = 0;
	}
	else if (camera->x > levelWidth - camera->w)
	{
		camera->x = levelWidth - camera->w;
	}
	if (camera->y < 0)
	{
		camera->y = 0;
	}
	else if (camera->y > levelHeight - camera->h)
	{
		camera->y = levelHeight - camera->h;
	}
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
		gameWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if (gameWindow == nullptr)
		{
			printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
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
	SDL_Quit();
}
