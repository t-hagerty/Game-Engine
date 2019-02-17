#include "GUIElement.h"
#include <SDL.h>
#include <string>
#pragma once
class Image :
	public GUIElement
{
public:
	Image(double x, double y, double width, double height, bool visible, std::string imageFilePath, int numberFrames, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer);
	Image(double x, double y, double width, double height, bool visible, SDL_Texture* imageTexture, int numberFrames, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer);
	~Image();
	void setAnimationFrame(int newFrame);
	int getAnimationFrame();
	void incrementAnimationFrame();
	void incrementAnimationFrame(int amount);
	bool render() override;
private:
	int FRAME_WIDTH;
	int FRAME_HEIGHT;
	int animationFrame = 0;
	int numberOfFrames;
};

