#include "Image.h"



Image::Image(double x, double y, double width, double height, bool visible, std::string imageFilePath, int numberFrames, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer)
	: GUIElement(x, y, width, height, visible, imageFilePath, targetSurface, targetRenderer)
{
	numberOfFrames = numberFrames;
}

Image::~Image()
{
	SDL_DestroyTexture(texture);
	delete rect;
}

void Image::setAnimationFrame(int newFrame)
{
	animationFrame = newFrame;
}

int Image::getAnimationFrame()
{
	return animationFrame;
}

void Image::incrementAnimationFrame()
{
	if (animationFrame++ >= numberOfFrames)
	{
		animationFrame = 0;
	}
}

void Image::incrementAnimationFrame(int amount)
{
	animationFrame += amount;
	if (animationFrame >= numberOfFrames)
	{
		animationFrame = 0;
	}
}

bool Image::render()
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
		//Set rendering space and render to screen
		SDL_Rect textureFrameClip = { 0, animationFrame * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
		//Render to screen
		SDL_RenderCopyEx(trgtRenderer, texture, &textureFrameClip, rect, 0.0, NULL, SDL_FLIP_NONE);
	}
	return success;
}

