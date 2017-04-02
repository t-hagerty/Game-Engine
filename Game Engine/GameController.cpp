#include "GameController.h"
#include <iostream>
#include "Entity.h"
#include "Rectangle.h"


GameController::GameController(GameModel* m, GameView* v)
{
	model = m;
	view = v;
	model->addEntity(new Rectangle(10, 10, 10, 5, 1, 0));
	model->addEntity(new Rectangle(20, 20, 480, 355, -1, -0.2));
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
	view->renderClear();
	view->renderTileMap(model->getTileMap(), model->getMapRows(), model->getMapCols(), model->getTileSize());
	Entity* p = model->getPlayer();
	view->renderRectangle(p->getPosX(), p->getPosY(), p->getWidth(), p->getHeight());
	for(int i = 0; i < model->getNumberOfEntities(); i++)
	{
		Entity* e = model->getEntity(i);
		view->renderRectangle(e->getPosX(), e->getPosY(), e->getWidth(), e->getHeight());
	}
	view->renderUpdate();
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
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
				view->close();
				return;
			}
			//User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_s:
					model->saveMap("testMap");
					break;
				default:
					break;
				}
			}
		}
		//keystates are updated every time SDL_Pollevent is called
		//If we check them inside the Pollevent loop though, keystates will be handled multiple times!
		const Uint8 *state = SDL_GetKeyboardState(NULL); //returns a pointer to array of key states when NULL is passed as parameter
		movePlayer(state);

		//update game logic/model
		update(delta); //<- all time related values must be multiplied by delta
		/*std::cout << "X Pos: ";
		std::cout << model->getPlayer()->getPosX();
		std::cout << " Y Pos: ";
		std::cout << model->getPlayer()->getPosY();
		std::cout << "\n";*/

		//render
		render();

		//Subtract current time from time when current iteration started will give us (time updates took in ms) * -1
		//Adding OPTIMAL_MS_PER_FRAME gives us the number of milliseconds left to delay to give us our TARGET_FPS
		long delay = lastLoopTime - SDL_GetTicks() + OPTIMAL_MS_PER_FRAME;
		if(delay >= 0) //if delay is less than zero, time taken for this loop was longer than optimal, continue ASAP
		{
			SDL_Delay(delay);
		}
	}
}

void GameController::movePlayer(const Uint8* keyStates) const
{
	model->getPlayer()->setVelocityX(0);
	model->getPlayer()->setVelocityY(0);
	if (keyStates[SDL_SCANCODE_UP])
	{
		model->getPlayer()->setVelocityY(-1);
	}
	else if (keyStates[SDL_SCANCODE_DOWN])
	{
		model->getPlayer()->setVelocityY(1);
	}
	if (keyStates[SDL_SCANCODE_LEFT])
	{
		model->getPlayer()->setVelocityX(-1);
	}
	else if (keyStates[SDL_SCANCODE_RIGHT])
	{
		model->getPlayer()->setVelocityX(1);
	}
}

