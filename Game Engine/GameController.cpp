#include "GameController.h"
#include <iostream>
#include "Entity.h"
#include "Enemy.h"


GameController::GameController(GameModel* m, GameView* v)
{
	model = m;
	view = v;
	view->setButtonHandlers(std::bind(&GameController::restartLevel, this), std::bind(&GameController::goToEditor, this), std::bind(&GameController::goToMenu, this));
	gameLoop();
}

GameController::~GameController()
{
}

int GameController::getExitCondition()
{
	return exitCondition;
}

void GameController::update(double delta) const
{
	model->moveEntities(delta);
}

void GameController::render() const
{
	view->renderClear();
	view->positionCamera(model->getPlayer()->getCollisionBox());
	view->renderTileMap(model->getTileMap(), model->getMapRows(), model->getMapCols(), model->getTileSize(), fps);

	for(int i = 0; i < model->getNumberOfEntities(); i++)
	{
		Entity* e = model->getEntity(i);
		view->renderEntitySprite(e, fps);
	}
	view->renderPlayerInfo(model->getPlayer()->getHealth());
	view->renderGUIElements();
	view->renderUpdate();
}

void GameController::mouseEventHandler(SDL_Event* e)
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
			return;
		}
	}
}

void GameController::restartLevel()
{
	model->resetLevel();
	view->setIsPaused(false);
}

void GameController::goToEditor()
{
	exitCondition = EDITOR;
	quitLoop = true;
}

void GameController::goToMenu()
{
	exitCondition = MAIN_MENU;
	quitLoop = true;
}

/*
 *Partially adapted from a wonderful game loop tutorial at http://www.java-gaming.org/index.php?topic=24220.0
*/
void GameController::gameLoop()
{
	long lastFpsTime = 0; //Keeps track of milliseconds since we last determined the FPS; should be reset to zero every second
	long lastLoopTime = SDL_GetTicks(); //SDL_GetTicks() returns a value in milliseconds
	const int TARGET_FPS = 60;
	const int OPTIMAL_MS_PER_FRAME = 1000 / TARGET_FPS; //1000 milliseconds in a second, divide by the target frames per second gives optimal milliseconds per frame

	//game loop
	while( !quitLoop)
	{
		long now = SDL_GetTicks();
		long updateLength = now - lastLoopTime;
		lastLoopTime = now;
		double delta = updateLength / static_cast<double>(OPTIMAL_MS_PER_FRAME); //delta = 1 when exactly 60 FPS, delta < 1 when over 60 FPS, delta > 1 when under 60 FPS

		//update frame counter
		lastFpsTime += updateLength;
 		fps++;
		//update FPS counter if a second has passed since last recorded
		if(lastFpsTime >= 1000)
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
				case SDL_WINDOWEVENT_MINIMIZED:
					if (!view->getIsPaused())
					{
						view->toggleMenu();
					}
					break;
				}
			}
		}
		//keystates are updated every time SDL_Pollevent is called
		//If we check them inside the Pollevent loop though, keystates will be handled multiple times!
		const Uint8 *state = SDL_GetKeyboardState(NULL); //returns a pointer to array of key states when NULL is passed as parameter
		movePlayer(state);
		
		//Safety check, if game hangs for a long time for some reason and delta is a large number, entities could pass right through things when game starts running again, so ignore this loop
		if (delta <= 2.5) //about <= 15 fps
		{
			if (!view->getIsPaused())
			{
				//update game logic/model
				update(delta); //<- all time related values must be multiplied by delta
			}
			if (model->getIsGameOver())
			{
				view->isGameOverScreen(true);
			}
			else if (view->getIsGameOverScreen()) //if model says it's not game over but view still says it is, we have restarted the level
			{
				view->isGameOverScreen(false);
				continue; //Before I added this, knockback from death was being carried over into the retry of the level
			}
			render();
		}
		//Subtract current time from time when current iteration started will give us (time updates took in ms) * -1
		//Adding OPTIMAL_MS_PER_FRAME gives us the number of milliseconds left to delay to give us our TARGET_FPS
		long delay = lastLoopTime - SDL_GetTicks() + OPTIMAL_MS_PER_FRAME;
		if(delay >= 0) //if delay is less than zero, time taken for this loop was longer than optimal, continue ASAP
		{
			SDL_Delay(delay);
		}
	}
}

void GameController::movePlayer(const Uint8* keyStates)
{
	if (keyStates[SDL_SCANCODE_UP] && keyStates[SDL_SCANCODE_DOWN])
	{
		if (isUpAlreadyPressed)
		{
			model->getPlayer()->setVerticalMovementKeyPress(1);
		}
		else
		{
			model->getPlayer()->setVerticalMovementKeyPress(-1);
		}
	}
	else if (keyStates[SDL_SCANCODE_UP])
	{
		model->getPlayer()->setVerticalMovementKeyPress(-1);
		isUpAlreadyPressed = true;
	}
	else if (keyStates[SDL_SCANCODE_DOWN])
	{
		model->getPlayer()->setVerticalMovementKeyPress(1);
		isUpAlreadyPressed = false;
	}
	else
	{
		model->getPlayer()->setVerticalMovementKeyPress(0);
		isUpAlreadyPressed = false;
	}
	if (keyStates[SDL_SCANCODE_LEFT] && keyStates[SDL_SCANCODE_RIGHT])
	{
		if (isLeftAlreadyPressed)
		{
			model->getPlayer()->setHorizontalMovementKeyPress(1);
		}
		else 
		{
			model->getPlayer()->setHorizontalMovementKeyPress(-1);
		}
	}
	else if (keyStates[SDL_SCANCODE_LEFT])
	{
		model->getPlayer()->setHorizontalMovementKeyPress(-1);
		isLeftAlreadyPressed = true;
	}
	else if (keyStates[SDL_SCANCODE_RIGHT])
	{
		model->getPlayer()->setHorizontalMovementKeyPress(1);
		isLeftAlreadyPressed = false;
	}
	else
	{
		model->getPlayer()->setHorizontalMovementKeyPress(0);
		isLeftAlreadyPressed = false;
	}
}

