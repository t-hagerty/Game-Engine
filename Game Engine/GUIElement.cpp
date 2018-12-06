#include "GUIElement.h"


GUIElement::GUIElement(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface * targetSurface, SDL_Renderer* targetRenderer)
{
	rect = new SDL_Rect();
	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
	isVisible = visible;
	trgtSurface = targetSurface;
	trgtRenderer = targetRenderer;
	loadTexture(targetSurface, targetRenderer, imageFilePath);
}

GUIElement::~GUIElement() {}

bool GUIElement::loadTexture(SDL_Surface* targetSurface, SDL_Renderer* targetRenderer, std::string filePath)
{
	SDL_Surface* optimizedImage = nullptr;

	SDL_Surface* loadedImage = SDL_LoadBMP(filePath.c_str());
	if (loadedImage == nullptr)
	{
		printf("Unable to load image %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
		return false;
	}
	else
	{
		optimizedImage = SDL_ConvertSurface(loadedImage, targetSurface->format, 0); //convert to image on surface to pixel format of the game's screen surface for optimization
		if (optimizedImage == nullptr)
		{
			printf("Unable to optimize image %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
			return false;
		}

		SDL_FreeSurface(loadedImage); //free memory of loadedImage
		SDL_SetColorKey(optimizedImage, SDL_TRUE, SDL_MapRGB(optimizedImage->format, 0xFF, 0, 0xFF));
		texture = SDL_CreateTextureFromSurface(targetRenderer, optimizedImage);
		if (texture == nullptr)
		{
			printf("Unable to create texture from %s. SDL Error: %s\n", filePath.c_str(), SDL_GetError());
			return false;
		}

		SDL_FreeSurface(optimizedImage);
	}

	return true;
}
void GUIElement::setTexture(SDL_Texture * newTexture)
{
	texture = newTexture;
}
SDL_Texture * GUIElement::getTexture()
{
	return texture;
}
void GUIElement::setRect(SDL_Rect * newRect)
{
	rect = newRect;
}
SDL_Rect * GUIElement::getRect()
{
	return rect;
}
void GUIElement::setPosX(double newX)
{
	rect->x = newX;
}
double GUIElement::getPosX()
{
	return rect->x;
}
void GUIElement::setPosY(double newY)
{
	rect->y = newY;
}
double GUIElement::getPosY()
{
	return rect->y;
}
void GUIElement::setWidth(double newWidth)
{
	rect->w = newWidth;
}
double GUIElement::getWidth()
{
	return rect->w;
}
void GUIElement::setHeight(double newHeight)
{
	rect->h = newHeight;
}
double GUIElement::getHeight()
{
	return rect->h;
}
void GUIElement::setIsVisible(bool visible)
{
	isVisible = visible;
}

bool GUIElement::getIsVisible()
{
	return isVisible;
}

void GUIElement::toggleVisibility()
{
	isVisible = !isVisible;
}

bool GUIElement::render()
{
	bool success = true;
	if (!isVisible)
	{
		return success;
	}
	else if (texture == nullptr)
	{
		printf("Unable to render GUIElement: Missing texture!\n");
		success = false;
	}
	else
	{
		SDL_RenderCopy(trgtRenderer, texture, NULL, rect);
	}
	return success;
}
