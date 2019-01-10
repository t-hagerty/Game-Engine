#include "EditorView.h"



EditorView::EditorView(int levelW, int levelH, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer) :
	View(levelW, levelH, windowW, windowH, window, screen, renderer)
{
	
	for (int i = 0; i < tileSet.size(); i++)
	{
		tileAnimationSynchronizer.insert(tileAnimationSynchronizer.end(), new Tile(0, 0, 0, i, false, false, nullptr));
	}
	tileSet.insert(tileSet.end(), loadTexture("spritesheets/player_walking.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("spritesheets/enemy_walking.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("spritesheets/arrow.bmp"));
	SDL_UpdateWindowSurface(gameWindow);

	camera->x = 0;
	camera->y = 0;
	initGUI();
}


EditorView::~EditorView()
{
	for (Tile* t : tileAnimationSynchronizer)
	{
		delete t;
	}
}

std::tuple<int, int> EditorView::convertScreenCoordsToModel(int mouseX, int mouseY)
{
	int resultX = mouseX - selectionMenu->getWidth() + camera->x;
	int resultY = mouseY + camera->y;
	return std::make_tuple(resultX, resultY);
}

void EditorView::setButtonHandlers(EventHandler testButtonHandler, EventHandler gameButtonHandler, EventHandler mainMenuButtonHandler)
{
	testButton->setEventHandler(testButtonHandler);
	gameButton->setEventHandler(gameButtonHandler);
	mainMenuButton->setEventHandler(mainMenuButtonHandler);
}

void EditorView::renderTileMap(std::vector<Tile*> map, int rows, int cols, int tileSize, int frame)
{
	if (frame == 0 || frame == 10 || frame == 20 || frame == 30 || frame == 40 || frame == 50)
	{
		incrementTileAnimationSynchronizer();
	}
	for (Tile* t : map)
	{
		if (SDL_HasIntersection(t->getTileSpace(), camera))
		{
			//Set rendering space and render to screen
			SDL_Rect renderQuad = { ((t->getTileSpace()->x - camera->x) * zoomScale) + selectionMenuWidth, (t->getTileSpace()->y - camera->y) * zoomScale , t->getTileSpace()->w * zoomScale, t->getTileSpace()->h * zoomScale };
			SDL_Rect textureFrameClip = { 0, getAnimationFrameForTile(t->getType()) * t->getTextureFrameHeight(), t->getTextureFrameWidth(), t->getTextureFrameHeight() };
			//Render to screen
			SDL_RenderCopyEx(gameRenderer, tileSet.at(t->getType()), &textureFrameClip, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
		}
	}
}

void EditorView::renderEntitySprite(Entity * e, int frame)
{
	if (e->getSpriteSheet() == nullptr)
	{
		e->setSpriteSheet(loadTexture(e->getSpriteFilePath()));
	}
	if (SDL_HasIntersection(e->getCollisionBox(), camera))
	{
		SDL_Rect spriteSheetClip = { 0, 0, e->getSpriteWidth(), e->getSpriteHeight() };
		SDL_Rect fillRect = { ((e->getPosX() - camera->x) * zoomScale) + selectionMenuWidth, (e->getPosY() - camera->y) * zoomScale , e->getWidth() * zoomScale , e->getHeight() * zoomScale };
		SDL_RenderCopy(gameRenderer, e->getSpriteSheet(), &spriteSheetClip, &fillRect);
	}
}

void EditorView::moveCameraLeft()
{
	moveCamera(-5, 0);
}

void EditorView::moveCameraRight()
{
	moveCamera(5, 0);
}

void EditorView::moveCameraUp()
{
	moveCamera(0, -5);
}

void EditorView::moveCameraDown()
{
	moveCamera(0, 5);
}

void EditorView::setSelectionButtonHandlers(EventHandler handler)
{
	for (Button* b : selectionMenu->getButtons())
	{
		b->setEventHandler(handler);
	}
}


void EditorView::moveCamera(int xChange, int yChange)
{
	camera->x += xChange;
	camera->y += yChange;
	//Keep the camera in bounds
	if (camera->x < 0)
	{
		camera->x = 0;
	}
	else if (camera->x > levelWidth - camera->w + selectionMenuWidth)
	{
		camera->x = levelWidth - camera->w + selectionMenuWidth;
	}
	if (camera->y < 0)
	{
		camera->y = 0;
	}
	else if (camera->y > levelHeight - camera->h)
	{
		camera->y = levelHeight - camera->h;
	}
}

bool EditorView::initGUI()
{
	bool success = true;
	double width = 50;
	if (50 < windowWidth / 15)
	{
		width = windowWidth / 15;
	}
	double height = 20;
	if (20 < windowHeight / 15)
	{
		height = windowHeight / 15;
	}
	double posX = windowWidth - width;
	double posY = 0;
	menuButton = new Button(posX, posY, width, height, true, "default_button.bmp", gScreenSurface, gameRenderer, "MENU", std::bind(&EditorView::toggleMenu, this));
	addButton(menuButton);
	menu = new ButtonMenu(225, 100, 150, 200, false, "menu.bmp", gScreenSurface, gameRenderer, 150, 10, 8, 1);
	gui.push_back(menu);
	testButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "TEST", nullptr);
	addButton(testButton);
	menu->addButton(testButton);
	settingsButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "SETTINGS", nullptr);
	addButton(settingsButton);
	menu->addButton(settingsButton);
	gameButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "GAME", nullptr);
	addButton(gameButton);
	menu->addButton(gameButton);
	mainMenuButton = new Button(0, 0, 200, 100, false, "default_button.bmp", gScreenSurface, gameRenderer, "MAIN MENU", nullptr);
	addButton(mainMenuButton);
	menu->addButton(mainMenuButton);
	selectionMenu = new ButtonMenu(0, 0, selectionMenuWidth, windowHeight, true, "menu.bmp", gScreenSurface, gameRenderer, 32, 10, 10, 3);
	populateSelectionMenu(); //adding the buttons to the selection menu halves the fps for some reason
	gui.push_back(selectionMenu); 
	return success;
}

int EditorView::getAnimationFrameForTile(int type)
{
	if (type < tileAnimationSynchronizer.size())
	{
		return tileAnimationSynchronizer[type]->getAnimationFrame();
	}
	return 0;
}

void EditorView::incrementTileAnimationSynchronizer()
{
	for (Tile* t : tileAnimationSynchronizer)
	{
		t->incrementAnimationFrame();
	}
}

void EditorView::populateSelectionMenu()
{
	for (int i = 0; i < static_cast<int>(tileSet.size()); i++)
	{
		Button* b = new Button(0, 0, 32, 32, true, "default_button.bmp", gScreenSurface, gameRenderer, " ", nullptr);
		b->setTexture(tileSet[i]);
		b->setEventArg(i);
		addButton(b);
		selectionMenu->addButton(b);
	}
}
