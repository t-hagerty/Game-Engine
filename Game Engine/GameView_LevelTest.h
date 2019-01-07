#pragma once
#include "GameView.h"
class GameView_LevelTest :
	public GameView
{
public:
	GameView_LevelTest(int levelWidth, int levelHeight, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer);
	~GameView_LevelTest();
	void setButtonHandlers(EventHandler retryButtonHandler, EventHandler editorButtonHandler, EventHandler mainMenuButtonHandler) override;
private:
	bool initGUI() override;
};

