#include "View.h"

View::View(int levelW, int levelH, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer)
{
	gameWindow = window;
	gScreenSurface = screen;
	gameRenderer = renderer;

	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_single.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_bottom_end.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_left_end.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_bottom_left_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_top_end.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_vertical.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_top_left_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_vertical_left.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_right_end.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_bottom_right_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_horizontal.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_horizontal_bottom.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_top_right_corner.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_vertical_right.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_horizontal_top.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/wall_filled.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/floor.bmp"));
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
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/door.bmp"));
	tileSet.insert(tileSet.end(), loadTexture("map_tiles/ladder.bmp"));

	levelWidth = levelW;
	levelHeight = levelH;
	windowWidth = windowW;
	windowHeight = windowH;
	camera->h = windowHeight;
	camera->w = windowWidth;
}


View::~View()
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

SDL_Texture * View::loadTexture(std::string filePath)
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

SDL_Surface * View::loadImage(std::string filePath) const
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

int View::getLevelWidth()
{
	return levelWidth;
}

int View::getLevelHeight()
{
	return levelHeight;
}

void View::setWindowWidth(int newWidth)
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

int View::getWindowWidth() const
{
	return windowWidth;
}

void View::setWindowHeight(int newHeight)
{
	if (windowHeight != newHeight)
	{
		if (zoomScale != 1)
		{
			camera->h += ((newHeight - windowHeight) / zoomScale);
		}
		else
		{
			camera->h = newHeight;
		}
		windowHeight = newHeight;
	}
}

int View::getWindowHeight() const
{
	return windowHeight;
}

SDL_Window * View::getWindow()
{
	return gameWindow;
}

SDL_Surface * View::getSurface()
{
	return gScreenSurface;
}

SDL_Renderer * View::getRenderer()
{
	return gameRenderer;
}

void View::setZoomScale(float newScale)
{
	zoomScale = newScale;
	camera->w = camera->w / zoomScale;
	camera->h = camera->h / zoomScale;
}

float View::getZoomScale()
{
	return zoomScale;
}

void View::setIsPaused(bool paused)
{
	isPaused = paused;
	if (!isPaused)
	{
		menu->setIsVisible(paused);
	}
}

bool View::getIsPaused()
{
	return isPaused;
}

void View::renderClear(int red, int green, int blue, int alpha) const
{
	//Clear screen
	SDL_SetRenderDrawColor(gameRenderer, red, green, blue, alpha);
	SDL_RenderClear(gameRenderer);
}

void View::renderUpdate() const
{
	//Update screen
	SDL_RenderPresent(gameRenderer);
}

void View::renderTileMap(std::vector<Tile*> map, int rows, int cols, int tileSize, int frame)
{
	for (Tile* t : map)
	{
		if (SDL_HasIntersection(t->getTileSpace(), camera))
		{
			//Set rendering space and render to screen
			SDL_Rect renderQuad = { (t->getTileSpace()->x - camera->x) * zoomScale , (t->getTileSpace()->y - camera->y) * zoomScale , t->getTileSpace()->w * zoomScale, t->getTileSpace()->h * zoomScale };
			SDL_Rect textureFrameClip = { 0, t->getAnimationFrame() * t->getTextureFrameHeight(), t->getTextureFrameWidth(), t->getTextureFrameHeight() };
			//Render to screen
			SDL_RenderCopyEx(gameRenderer, tileSet.at(t->getType()), &textureFrameClip, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
		}
		if ((frame == 0 || frame == 10 || frame == 20 || frame == 30 || frame == 40 || frame == 50) && !isPaused)
		{
			t->incrementAnimationFrame();
		}
	}
}

void View::renderRectangle(double posX, double posY, int width, int height) const
{
	SDL_Rect fillRect = { posX , posY , width , height };
	fillRect.x -= camera->x;
	fillRect.y -= camera->y;
	SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderFillRect(gameRenderer, &fillRect);
}

void View::renderEntitySprite(Entity * e, int frame)
{
	if (e->getSpriteSheet() == nullptr)
	{
		e->setSpriteSheet(loadTexture(e->getSpriteFilePath()));
	}
	if (SDL_HasIntersection(e->getCollisionBox(), camera))
	{
		SDL_Rect spriteSheetClip = { e->getAnimationFrame() * e->getSpriteWidth(),
									e->getSpriteDirection() * e->getSpriteHeight(),
									e->getSpriteWidth(), e->getSpriteHeight() };
		SDL_Rect fillRect = { (e->getPosX() - camera->x) * zoomScale, (e->getPosY() - camera->y) * zoomScale , e->getWidth() * zoomScale , e->getHeight() * zoomScale };
		SDL_RenderCopy(gameRenderer, e->getSpriteSheet(), &spriteSheetClip, &fillRect);
	}
	if ((e->getVelocityX() != 0 || e->getVelocityY() != 0))
	{
		if (e->getKnockbackTimer() == 0 && (frame == 0 || frame == 15 || frame == 30 || frame == 45) && !isPaused)
		{
			e->incrementAnimationFrame();
		}
	}
	else
	{
		e->setAnimationFrame(0);
	}
}

SDL_Texture* View::createTextTexture(std::string text)
{
	if (text == "")
	{
		return nullptr;
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
	SDL_Texture* message = nullptr;
	if (!(messageSurface = TTF_RenderText_Solid(font, s.str().c_str(), textColor)))
	{
		printf("Text could not display. TTF Error: %s\n", TTF_GetError());
	}
	else
	{
		message = SDL_CreateTextureFromSurface(gameRenderer, messageSurface);
	}
	SDL_FreeSurface(messageSurface);
	TTF_CloseFont(font);
	return message;
}

void View::renderText(std::string text, SDL_Rect * textRect)
{
	SDL_Texture* message = createTextTexture(text);
	if (message = nullptr)
	{
		return;
	}
	SDL_RenderCopy(gameRenderer, message, NULL, textRect);
	SDL_DestroyTexture(message);
}

void View::renderButtons()
{
	for (Button* aButton : buttons)
	{
		if (aButton->getIsVisible())
		{
			renderAButton(aButton);
		}
	}
}

void View::renderAButton(Button * aButton)
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

void View::renderImage(Image * anImage)
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

void View::renderGUIElements()
{
	for (GUIElement* g : gui)
	{
		g->render();
	}
}

std::vector<Button*> View::getButtons()
{
	return buttons;
}

void View::addButton(Button * aButton)
{
	buttons.push_back(aButton);
	gui.push_back(aButton);
}

Button * View::addButton(double posX, double posY, double width, double height, bool isVisible, std::string filePath, std::string buttonText, EventHandler eventHandler)
{
	Button* aButton = new Button(posX, posY, width, height, isVisible, filePath, gScreenSurface, gameRenderer, buttonText, eventHandler);
	addButton(aButton);
	return aButton;
}

void View::toggleMenu()
{
	isPaused = !isPaused;
	menu->toggleVisibility();
	renderUpdate();
}
