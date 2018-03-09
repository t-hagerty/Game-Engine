#include <SDL.h>
#include <string>
#pragma once
class Image
{
public:
	Image(double x, double y, double width, double height, bool visible, std::string imageFilePath);
	~Image();
	void setImageRect(SDL_Rect* newRect);
	SDL_Rect* getImageRect();
	void setTexture(SDL_Texture* newTexture);
	SDL_Texture* getTexture();
	void setIsVisible(bool visible);
	bool getIsVisible();
	void toggleVisibility();
	void setImageFilePath(std::string newFilePath);
	std::string getImageFilePath();
private:
	SDL_Rect* imageRect;
	SDL_Texture* imageTexture;
	bool isVisible;
	std::string imageFile;
};

