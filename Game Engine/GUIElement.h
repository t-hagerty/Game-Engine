#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include <string>
#pragma once
class GUIElement
{
public:
	GUIElement(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer);
	virtual ~GUIElement() = 0;
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
	virtual void setIsVisible(bool visible);
	bool getIsVisible();
	virtual void toggleVisibility();
	virtual bool render();
protected:
	SDL_Texture* texture = nullptr;
	SDL_Rect* rect;
	SDL_Surface* trgtSurface;
	SDL_Renderer* trgtRenderer;
	bool isVisible;
};

