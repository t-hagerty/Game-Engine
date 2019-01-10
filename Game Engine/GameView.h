#pragma once
#include "View.h"

class GameView :
	public View
{
public:
	GameView(int levelWidth, int levelHeight, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer);
	~GameView();
	void renderPlayerInfo(double playerHealth);
	void positionCamera(SDL_Rect* playerBox) const;
	virtual void setButtonHandlers(EventHandler retryButtonHandler, EventHandler editorButtonHandler, EventHandler mainMenuButtonHandler);
	void settingsButtonPressed();
	void isGameOverScreen(bool isGameOver);
	bool getIsGameOverScreen();
protected:
	Button* editorButton;
	Button* restartButton;
	Button* retryButton;
	HealthBar* hpBar;

	virtual bool initGUI() override;
};

