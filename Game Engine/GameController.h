#pragma once
#include "GameModel.h"
#include "GameView.h"
#include <SDL.h>
#undef main

class GameController
{
public:
	GameController();
	GameController(GameModel* m, GameView* v);
	~GameController();
	SDL_Event e;
	int getExitCondition();
protected:
	int fps = 0;
	void gameLoop();
	void movePlayer(const Uint8* keyStates);
	void update(double delta) const;
	void render() const;
	void mouseEventHandler(SDL_Event* e);
	void restartLevel();
	void goToEditor();
	virtual void goToMenu();
	GameModel* model;
	GameView* view;
	//Key press surfaces constants
	enum KeyPresses
	{
		KEY_PRESS_DEFAULT,
		KEY_PRESS_UP,
		KEY_PRESS_DOWN,
		KEY_PRESS_LEFT,
		KEY_PRESS_RIGHT,
		KEY_PRESS_TOTAL
	};
	bool isUpAlreadyPressed = false;
	bool isLeftAlreadyPressed = false;
	enum ExitConditions
	{
		QUIT,
		GAME,
		EDITOR,
		MAIN_MENU
	};
	int exitCondition = QUIT;
	bool quitLoop = false; //For terminating the game loop
};

