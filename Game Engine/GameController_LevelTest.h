#pragma once
#include "GameController.h"
class GameController_LevelTest :
	public GameController
{
public:
	GameController_LevelTest(GameModel* m, GameView* v);
	~GameController_LevelTest();
	void goToMenu() override;
};

