#include "Image.h"



Image::Image(double x, double y, double width, double height, bool visible, std::string imageFilePath)
{
	imageRect = new SDL_Rect();
	imageRect->x = x;
	imageRect->y = y;
	imageRect->w = width;
	imageRect->h = height;
	isVisible = visible;
	imageFile = imageFilePath;
}

Image::~Image()
{
}

void Image::setImageRect(SDL_Rect * newRect)
{
	delete imageRect;
	imageRect = newRect;
}

SDL_Rect * Image::getImageRect()
{
	return imageRect;
}

void Image::setTexture(SDL_Texture * newTexture)
{
	imageTexture = newTexture;
}

SDL_Texture * Image::getTexture()
{
	return imageTexture;
}

void Image::setIsVisible(bool visible)
{
	isVisible = visible;
}

bool Image::getIsVisible()
{
	return isVisible;
}

void Image::toggleVisibility()
{
	isVisible = !isVisible;
}

void Image::setImageFilePath(std::string newFilePath)
{
	imageFile = newFilePath;
}

std::string Image::getImageFilePath()
{
	return imageFile;
}
