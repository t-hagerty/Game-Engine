#include "EditorView.h"



EditorView::EditorView(int levelW, int levelH, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer)
{
	gameWindow = window;
	gScreenSurface = screen;
	gameRenderer = renderer;

	tileSet.insert(tileSet.end(), loadTexture("map_tiles/floor.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_bottom_left_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_bottom_right_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_top_left_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_top_right_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_horizontal.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_vertical.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/grass.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/barrier.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/down_treadmill.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/right_treadmill.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/left_treadmill.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/up_treadmill.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/ice.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/mud.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/pit.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/lava.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/spikes.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("spritesheets/player_walking.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("spritesheets/enemy_walking.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("spritesheets/arrow.bmp"));
	SDL_UpdateWindowSurface(gameWindow);

	camera->x = 0;
	camera->y = 0;
	camera->h = windowHeight;
	camera->w = windowWidth;
	levelWidth = levelW;
	levelHeight = levelH;
	windowWidth = windowW;
	windowHeight = windowH;
	initGUI();
}


EditorView::~EditorView()
{
	renderClear(0xFF, 0xFF, 0xFF, 0xFF);
	delete camera;
	for (GUIElement* g : gui)
	{
		delete g;
	}
	for (SDL_Texture* t : tileSet)
	{
		SDL_DestroyTexture(t);
	}
}

void EditorView::setWindowWidth(int newWidth)
{
	if (windowWidth != newWidth)
	{
		if (zoomScale != 1)
		{
			camera->w += ((newWidth - windowWidth) / zoomScale);
		}
		else
		{
			camera->w = newWidth;
		}
		windowWidth = newWidth;
	}
}

int EditorView::getWindowWidth() const
{
	return windowWidth;
}

void EditorView::setWindowHeight(int newHeight)
{
	if (windowHeight != newHeight)
	{
		if (zoomScale != 1)
		{
			camera->h += ((newHeight - windowHeight) / zoomScale);
			//printf("New height: %d, new camera height: %d \n", newHeight, camera->h);
		}
		else
		{
			camera->h = newHeight;
		}
		windowHeight = newHeight;
	}
}

int EditorView::getWindowHeight() const
{
	return windowHeight;
}

void EditorView::setZoomScale(float newScale)
{
	zoomScale = newScale;
	camera->w = camera->w / zoomScale;
	camera->h = camera->h / zoomScale;
}

float EditorView::getZoomScale()
{
	return zoomScale;
}

bool EditorView::isPaused()
{
	return ((menu->getIsVisible()) ? true : false);
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

std::vector<Button*> EditorView::getButtons()
{
	return buttons;
}

void EditorView::renderClear(int red, int green, int blue, int alpha) const
{
	//Clear screen
	SDL_SetRenderDrawColor(gameRenderer, red, green, blue, alpha);
	SDL_RenderClear(gameRenderer);
}

void EditorView::renderUpdate() const
{
	//Update screen
	SDL_RenderPresent(gameRenderer);
}

void EditorView::renderTileMap(std::vector<Tile*> map, int rows, int cols, int tileSize, int frame)
{
	for (Tile* t : map)
	{
		if (SDL_HasIntersection(t->getTileSpace(), camera))
		{
			//Set rendering space and render to screen
			SDL_Rect renderQuad = { ((t->getTileSpace()->x - camera->x) * zoomScale) + selectionMenuWidth, (t->getTileSpace()->y - camera->y) * zoomScale , t->getTileSpace()->w * zoomScale, t->getTileSpace()->h * zoomScale };
			SDL_Rect textureFrameClip = { 0, t->getAnimationFrame() * t->getTextureFrameHeight(), t->getTextureFrameWidth(), t->getTextureFrameHeight() };
			//Render to screen
			SDL_RenderCopyEx(gameRenderer, tileSet.at(t->getType()), &textureFrameClip, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
		}
		if (frame == 0 || frame == 10 || frame == 20 || frame == 30 || frame == 40 || frame == 50)
		{
			t->incrementAnimationFrame();
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

void EditorView::renderText(std::string text, SDL_Rect * textRect)
{
	if (text == "")
	{
		return;
	}
	std::stringstream s;
	s << text;

	TTF_Font* font = TTF_OpenFont("segoeui.ttf", 24);
	if (!font)
	{
		printf("TTF_OpenFont Error: %s\n", TTF_GetError());
	}
	SDL_Color textColor = { 0, 0, 0 };
	SDL_Surface* messageSurface;
	if (!(messageSurface = TTF_RenderText_Solid(font, s.str().c_str(), textColor)))
	{
		printf("Text could not display. TTF Error: %s\n", TTF_GetError());
	}
	else
	{
		SDL_Texture* message = SDL_CreateTextureFromSurface(gameRenderer, messageSurface);

		SDL_RenderCopy(gameRenderer, message, NULL, textRect);
		SDL_DestroyTexture(message);
	}
	SDL_FreeSurface(messageSurface);
	TTF_CloseFont(font);
}

void EditorView::renderButtons()
{
	for (Button* aButton : buttons)
	{
		if (aButton->getIsVisible())
		{
			renderAButton(aButton);
		}
	}
}

void EditorView::renderAButton(Button * aButton)
{
	if (aButton->getIsVisible())
	{
		SDL_Rect spriteSheetClip = { aButton->getButtonState() * aButton->getButtonImageWidth(),
			0,
			aButton->getButtonImageWidth(), aButton->getButtonImageHeight() };
		SDL_Rect renderRect = { aButton->getRect()->x , aButton->getRect()->y , aButton->getRect()->w , aButton->getRect()->h };
		SDL_RenderCopy(gameRenderer, aButton->getTexture(), &spriteSheetClip, &renderRect);
		renderText(aButton->getButtonText(), aButton->getRect());
	}
}

void EditorView::renderImage(Image * anImage)
{
	if (!anImage->getIsVisible())
	{
		return;
	}
	else
	{
		SDL_Rect renderRect = { anImage->getRect()->x , anImage->getRect()->y , anImage->getRect()->w , anImage->getRect()->h };
		SDL_RenderCopy(gameRenderer, anImage->getTexture(), NULL, &renderRect);
	}
}

void EditorView::renderGUIElements()
{
	for (GUIElement* g : gui)
	{
		g->render();
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

void EditorView::addButton(Button * aButton)
{
	buttons.push_back(aButton);
	gui.push_back(aButton);
}

Button * EditorView::addButton(double posX, double posY, double width, double height, bool isVisible, std::string filePath, std::string buttonText, EventHandler eventHandler)
{
	Button* aButton = new Button(posX, posY, width, height, isVisible, filePath, gScreenSurface, gameRenderer, buttonText, eventHandler);
	addButton(aButton);
	return aButton;
}

void EditorView::setSelectionButtonHandlers(EventHandler handler)
{
	for (Button* b : selectionMenu->getButtons())
	{
		b->setEventHandler(handler);
	}
}

void EditorView::toggleMenu()
{
	menu->toggleVisibility();
	renderUpdate();
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
	else if (camera->x > levelWidth - camera->w)
	{
		camera->x = levelWidth - camera->w;
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

SDL_Texture * EditorView::loadTexture(std::string filePath)
{
	SDL_Texture* texture = nullptr;

	SDL_Surface* textureImage = loadImage(filePath);
	if (textureImage == nullptr)
	{
		printf("Unable to load image %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
	}
	else
	{
		SDL_SetColorKey(textureImage, SDL_TRUE, SDL_MapRGB(textureImage->format, 0xFF, 0, 0xFF));
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		texture = SDL_CreateTextureFromSurface(gameRenderer, textureImage);
		if (texture == nullptr)
		{
			printf("Unable to create texture from %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(textureImage);
	}

	return texture;
}

SDL_Surface * EditorView::loadImage(std::string filePath) const
{
	SDL_Surface* optimizedImage = nullptr;

	SDL_Surface* loadedImage = SDL_LoadBMP(filePath.c_str());
	if (loadedImage == nullptr)
	{
		printf("Unable to load image %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
	}
	else
	{
		optimizedImage = SDL_ConvertSurface(loadedImage, gScreenSurface->format, 0); //convert to image on surface to pixel format of the game's screen surface for optimization
		if (optimizedImage == nullptr)
		{
			printf("Unable to optimize image %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedImage); //free memory of loadedImage
	}
	return optimizedImage;
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
