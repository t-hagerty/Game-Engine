#include <SDL.h>
#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"
#include "EditorView.h"
#include "EditorModel.h"
#include "EditorController.h"


int main()
{
	int windowHeight = 400;
	int windowWidth = 600;
	SDL_Window* gameWindow = nullptr;
	SDL_Surface* gScreenSurface = nullptr;
	SDL_Renderer* gameRenderer = nullptr;
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
				SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
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
	if (!success)
	{
		printf("Failed to initialize\n");
		return 0;
	}

	bool quit = false;
	enum ExitConditions
	{
		QUIT,
		GAME,
		EDITOR,
		MAIN_MENU
	};
	int router = GAME;
	GameModel* aGameModel;
	GameView* aGameView;
	GameController* aGameController;
	EditorModel* anEditorModel;
	EditorView* anEditorView;
	EditorController* anEditorController;
	while (!quit)
	{
		switch (router)
		{
		case QUIT:
			quit = true;
			break;
		case GAME:
			aGameModel =  new GameModel(2);
			aGameView = new GameView(aGameModel->getLevelWidth(), aGameModel->getLevelHeight(), windowWidth, windowHeight, gameWindow, gScreenSurface, gameRenderer);
			aGameController = new GameController(aGameModel, aGameView);
			router = aGameController->getExitCondition();
			delete aGameModel;
			delete aGameView;
			delete aGameController;
			break;
		case EDITOR:
			anEditorModel = new EditorModel();
			anEditorView = new EditorView(anEditorModel->getLevelWidth(), anEditorModel->getLevelHeight(), windowWidth, windowHeight, gameWindow, gScreenSurface, gameRenderer);
			anEditorController = new EditorController(anEditorModel, anEditorView);
			router = anEditorController->getExitCondition();
			delete anEditorModel;
			delete anEditorView;
			delete anEditorController;
			break;
		case MAIN_MENU:
			break;
		default:
			printf("ERROR: Invalid program routing state %d", router);
			quit = true;
			break;
		}
	}
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameWindow = nullptr;
	gameRenderer = nullptr;
	TTF_Quit();
	SDL_Quit();
	return 0;
}
