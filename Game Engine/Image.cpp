#include "Image.h"



Image::Image(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer)
	: GUIElement(x, y, width, height, visible, imageFilePath, targetSurface, targetRenderer)
{

}

Image::~Image()
{
	//delete texture;
	delete rect;
}

