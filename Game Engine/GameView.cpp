#include "GameView.h"
#include <cstdio>

/*
 * Game rendering using SDL was learned from http://lazyfoo.net/tutorials/SDL/index.php and some of the code here is currently borrowed from lessons at said site.
 */

GameView::GameView()
{
	if(!init())
	{
		printf("Failed to initialize\n");
	}
	else
	{
		SDL_UpdateWindowSurface(gameWindow);
	}
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

void GameView::renderRectangle(double posX, double posY, int width, int height) const
{
	SDL_Rect fillRect = { posX, posY, width, height};
	//printf("posx: %f, posy: %f, posx + width: %f, posy + height: %f", posX, posY, (posX + width), (posY + height));
	SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderFillRect(gameRenderer, &fillRect);
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
		}
	}
	return success;
}

void GameView::close()
{	
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameWindow = nullptr;
	gameRenderer = nullptr;
	SDL_Quit();
}
