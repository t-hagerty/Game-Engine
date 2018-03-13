#include <SDL.h>
#include <string>
#pragma once
class GUIElement
{
public:
	GUIElement(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer);
	~GUIElement();
	bool loadTexture(SDL_Surface* targetSurface, SDL_Renderer* targetRenderer, std::string filePath);
	void setTexture(SDL_Texture* newTexture);
	SDL_Texture* getTexture();
	void setRect(SDL_Rect* newRect);
	SDL_Rect* getRect();
	void setPosX(double newX);
	double getPosX();
	void setPosY(double newY);
	double getPosY();
	void setWidth(double newWidth);
	double getWidth();
	void setHeight(double newHeight);
	double getHeight();
	void setIsVisible(bool visible);
	bool getIsVisible();
	void toggleVisibility();
private:
	SDL_Texture* texture = nullptr;
	SDL_Rect* rect;
	bool isVisible;
};

