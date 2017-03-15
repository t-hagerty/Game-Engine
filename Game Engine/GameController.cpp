#include "GameController.h"
#include <iostream>
#include "Entity.h"
#include "Rectangle.h"


GameController::GameController(GameModel* m, GameView* v)
{
	model = m;
	view = v;
	model->setAreaHeight(view->getWindowHeight());
	model->setAreaWidth(view->getWindowWidth());
	model->addEntity(new Rectangle(10, 10, 20, 50, 0, 0));
	gameLoop();
}

GameController::~GameController()
{
}

void GameController::update(double delta) const
{
	model->moveEntities(delta);
}

void GameController::render() const
{
	Entity* e = model->getEntity(0);
	view->renderRectangle(e->getPosX(), e->getPosY(), e->getWidth(), e->getHeight());
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
	bool quit = false;

	//game loop
	while( !quit)
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
			//keystates are updated every time SDL_Pollevent is called
			const Uint8 *state = SDL_GetKeyboardState(NULL); //returns a pointer to array of key states when NULL is passed as parameter
			movePlayer(state);
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
				view->close();
				return;
			}
			//User presses a key
			//else if (e.type == SDL_KEYDOWN)
			//{
			//	//Select surfaces based on key press
			//	switch (e.key.keysym.sym)
			//	{
			//	case SDLK_UP:
			//		movePlayer(KEY_PRESS_UP);
			//		break;

			//	case SDLK_DOWN:
			//		movePlayer(KEY_PRESS_DOWN);
			//		break;

			//	case SDLK_LEFT:
			//		movePlayer(KEY_PRESS_LEFT);
			//		break;

			//	case SDLK_RIGHT:
			//		movePlayer(KEY_PRESS_RIGHT);
			//		break;

			//	default:
			//		movePlayer(KEY_PRESS_DEFAULT);
			//		break;
			//	}
			//}
		}

		//update game logic/model
		update(delta); //<- all time related values must be multiplied by delta

		//render
		render();

		//Subtract current time from time when current iteration started will give us (time updates took in ms) * -1
		//Adding OPTIMAL_MS_PER_FRAME gives us the number of milliseconds left to delay to give us our TARGET_FPS
		SDL_Delay(lastLoopTime - SDL_GetTicks() + OPTIMAL_MS_PER_FRAME);
	}
}

void GameController::movePlayer(const Uint8* keyStates) const
{
	model->getEntity(0)->setVelocityX(0);
	model->getEntity(0)->setVelocityY(0);
	if (keyStates[SDL_SCANCODE_UP])
	{
		model->getEntity(0)->setVelocityY(-1);
	}
	else if (keyStates[SDL_SCANCODE_DOWN])
	{
		model->getEntity(0)->setVelocityY(1);
	}
	if (keyStates[SDL_SCANCODE_LEFT])
	{
		model->getEntity(0)->setVelocityX(-1);
	}
	else if (keyStates[SDL_SCANCODE_RIGHT])
	{
		model->getEntity(0)->setVelocityX(1);
	}
}

