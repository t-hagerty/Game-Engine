#pragma once
#include "GameModel.h"
#include "GameView.h"
#include <SDL.h>
#undef main

class GameController
{
public:
	GameController(GameModel* m, GameView* v);
	~GameController();
	SDL_Event e;
private:
	int fps = 0;
	void gameLoop();
	void movePlayer(const Uint8* keyStates);
	void update(double delta) const;
	void render() const;
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
};

