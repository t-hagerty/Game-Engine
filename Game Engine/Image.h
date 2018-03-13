#include "GUIElement.h"
#include <SDL.h>
#include <string>
#pragma once
class Image :
	public GUIElement
{
public:
	Image(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer);
	~Image();
private:

};

