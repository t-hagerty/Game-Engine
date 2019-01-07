#include "GameController_LevelTest.h"



GameController_LevelTest::GameController_LevelTest(GameModel* m, GameView* v)
{
	model = m;
	view = v;
	view->setButtonHandlers(std::bind(&GameController_LevelTest::restartLevel, this), std::bind(&GameController_LevelTest::goToEditor, this), std::bind(&GameController_LevelTest::goToMenu, this));
	gameLoop();
}


GameController_LevelTest::~GameController_LevelTest()
{
}

void GameController_LevelTest::goToMenu()
{
	//TODO: Pop up some warning that unsaved level will be lost?
	exitCondition = MAIN_MENU;
	quitLoop = true;
}
