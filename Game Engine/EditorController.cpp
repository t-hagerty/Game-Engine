#include "EditorController.h"
#include "GameController_LevelTest.h"
#include "GameView_LevelTest.h"
#include "GameModel.h"
#include <iostream>

EditorController::EditorController(EditorModel * m, EditorView * v)
{
	using namespace std::placeholders; //for the _1 argument placeholder below
	model = m;
	view = v;
	view->setButtonHandlers(std::bind(&EditorController::goToTest, this), std::bind(&EditorController::goToGame, this), std::bind(&EditorController::goToMenu, this));
	view->setSelectionButtonHandlers(std::bind(&EditorModel::setSelectedTileType, model, _1));
	//view->setSelectionButtonHandlers(std::bind(&EditorController::setTileSelection, this, _1));
	gameLoop();
}

EditorController::~EditorController()
{
}

int EditorController::getExitCondition()
{
	return exitCondition;
}

void EditorController::gameLoop()
{
	long lastFpsTime = 0; //Keeps track of milliseconds since we last determined the FPS; should be reset to zero every second
	long lastLoopTime = SDL_GetTicks(); //SDL_GetTicks() returns a value in milliseconds
	const int TARGET_FPS = 60;
	const int OPTIMAL_MS_PER_FRAME = 1000 / TARGET_FPS; //1000 milliseconds in a second, divide by the target frames per second gives optimal milliseconds per frame

	//game loop
	while (!quitLoop)
	{
		long now = SDL_GetTicks();
		long updateLength = now - lastLoopTime;
		lastLoopTime = now;
		double delta = updateLength / static_cast<double>(OPTIMAL_MS_PER_FRAME); //delta = 1 when exactly 60 FPS, delta < 1 when over 60 FPS, delta > 1 when under 60 FPS

		//update frame counter
		lastFpsTime += updateLength;
		fps++;
		//update FPS counter if a second has passed since last recorded
		if (lastFpsTime >= 1000)
		{
			std::cout << "FPS: ";
			std::cout << fps;
			std::cout << "\n";
			lastFpsTime = 0;
			fps = 0;
		}

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quitLoop = true;
				return;
			}
			//User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_s:
					//model->saveMap("testMap");
					break;
				case SDLK_UP:
					view->moveCameraUp();
					break;
				case SDLK_DOWN:
					view->moveCameraDown();
					break;
				case SDLK_LEFT:
					view->moveCameraLeft();
					break;
				case SDLK_RIGHT:
					view->moveCameraRight();
					break;
				default:
					break;
				}
			}
			if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				mouseEventHandler(&e);
			}
			if (e.type == SDL_WINDOWEVENT)
			{
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					view->setWindowWidth(e.window.data1);
					view->setWindowHeight(e.window.data2);
					break;
				}
			}
		}
		//keystates are updated every time SDL_Pollevent is called
		//If we check them inside the Pollevent loop though, keystates will be handled multiple times!
		const Uint8 *state = SDL_GetKeyboardState(NULL); //returns a pointer to array of key states when NULL is passed as parameter

		//Safety check, if game hangs for a long time for some reason and delta is a large number, entities could pass right through things when game starts running again, so ignore this loop
		if (delta <= 2.5) //about <= 15 fps
		{
			//update game logic/model
			update(delta); //<- all time related values must be multiplied by delta
			render();
		}
		//Subtract current time from time when current iteration started will give us (time updates took in ms) * -1
		//Adding OPTIMAL_MS_PER_FRAME gives us the number of milliseconds left to delay to give us our TARGET_FPS
		long delay = lastLoopTime - SDL_GetTicks() + OPTIMAL_MS_PER_FRAME;
		if (delay >= 0) //if delay is less than zero, time taken for this loop was longer than optimal, continue ASAP
		{
			SDL_Delay(delay);
		}
	}
}

void EditorController::update(double delta) const
{
}

void EditorController::render() const
{
	view->renderClear();
	view->renderTileMap(model->getTileMap(), model->getMapRows(), model->getMapCols(), model->getTileSize(), fps);

	for (int i = 0; i < model->getNumberOfEntities(); i++)
	{
		Entity* e = model->getEntity(i);
		view->renderEntitySprite(e, fps);
	}
	view->renderGUIElements();
	view->renderUpdate();
}

void EditorController::mouseEventHandler(SDL_Event * e)
{
	//Get mouse position
	int x, y;
	SDL_GetMouseState(&x, &y);
	for (Button* aButton : view->getButtons())
	{
		if (!aButton->getIsVisible())
		{
			continue; //skip hidden buttons
		}
		aButton->setIsMouseOver(false);
		//Mouse is left of the button
		if (x < aButton->getPosX())
		{
			continue;
		}
		//Mouse is right of the button
		else if (x > aButton->getPosX() + aButton->getWidth())
		{
			continue;
		}
		//Mouse above the button
		else if (y < aButton->getPosY())
		{
			continue;
		}
		//Mouse below the button
		else if (y > aButton->getPosY() + aButton->getHeight())
		{
			continue;
		}

		switch (e->type)
		{
		case SDL_MOUSEBUTTONDOWN:
			aButton->setIsMouseDown(true);
			return;
		case SDL_MOUSEMOTION:
			aButton->setIsMouseOver(true);
			return;
		case SDL_MOUSEBUTTONUP:
			aButton->setIsMouseUp(true);
			aButton->triggerEvent();
			isMouseBeingDragged = false;
			highlightCurrentSelection();
			return;
		}
	}
	if ((e->type == SDL_MOUSEBUTTONDOWN || (isMouseBeingDragged && e->type == SDL_MOUSEMOTION)) && !view->isPaused())
	{
		isMouseBeingDragged = true;
		std::tuple<int, int> modelCoords = view->convertScreenCoordsToModel(x, y);
		model->clickTile(std::get<0>(modelCoords), std::get<1>(modelCoords));
	}
	else if (e->type == SDL_MOUSEBUTTONUP)
	{
		isMouseBeingDragged = false;
	}
}

void EditorController::goToTest()
{
	int router = EDITOR;
	if (model->getPlayer() == nullptr)
	{
		//TODO pop up some warning dialog to tell user they cant test without a player
		return;
	}
	GameModel* testLevel = new GameModel(model->getTileMap(), model->getMapRows(), model->getMapCols(), model->getEntities());
	GameView_LevelTest* testView = new GameView_LevelTest(view->getLevelWidth(), view->getLevelHeight(), view->getWindowWidth(), view->getWindowHeight(),
		view->getWindow(), view->getSurface(), view->getRenderer());
	GameController_LevelTest* testController = new GameController_LevelTest(testLevel, testView);
	router = testController->getExitCondition();
	delete testLevel;
	delete testView;
	delete testController;
	switch (router)
	{
	case EDITOR:
		view->toggleMenu();
		break;
	case MAIN_MENU:
		goToMenu();
		break;
	default:
		goToMenu();
		break;
	}
}

void EditorController::goToGame()
{
	exitCondition = GAME;
	quitLoop = true;
}

void EditorController::goToMenu()
{
	exitCondition = MAIN_MENU;
	quitLoop = true;
}

void EditorController::highlightCurrentSelection()
{
	int current = model->getSelectedTileType();
	for (Button* b : view->getButtons())
	{
		if (b->getEventArg() == current)
		{
			b->setIsHighlighted(true);
		}
		else
		{
			b->setIsHighlighted(false);
		}
	}
}
