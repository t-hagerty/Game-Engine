#include <SDL.h>
#include <string>
#include <functional>
#pragma once
class Button
{
public:
	typedef std::function<void()> EventHandler;
	Button(double x, double y, double width, double height, std::string imageFilePath = "default_button.bmp", std::string buttonText = "", EventHandler buttonEventHandler = NULL);
	~Button();
	void setPosX(double newPos);
	double getPosX();
	void setPosY(double newPos);
	double getPosY();
	void setWidth(double newWidth);
	double getWidth();
	void setHeight(double newHeight);
	double getHeight();
	void setImageFilePath(std::string newFilePath);
	std::string getImageFilePath();
	void setButtonTexture(SDL_Texture* newTexture);
	SDL_Texture* getButtonTexture();
	int getButtonImageWidth();
	int getButtonImageHeight();
	void setButtonText(std::string newText);
	std::string getButtonText();
	void setButtonrect(SDL_Rect* newRect);
	SDL_Rect* getButtonRect();
	void setIsVisible(bool visible);
	bool getIsVisible();
	void setIsMouseOver(bool mouseOver);
	bool getIsMouseOver();
	void setIsMouseDown(bool mouseDown);
	bool getIsMouseDown();
	void setIsMouseUp(bool mouseUp);
	bool getIsMouseUp();
	int getButtonState();
	void triggerEvent();
private:
	SDL_Rect* buttonRect;
	std::string imageFile;
	SDL_Texture* buttonTexture; //spritesheet 0 = default, 1 = mouseover, 2 = mousedown, 3 = mouseup
	const int BUTTON_IMAGE_WIDTH = 32;
	const int BUTTON_IMAGE_HEIGHT = 32;
	std::string text;
	bool isVisible;
	bool isMouseOver;
	bool isMouseDown;
	bool isMouseUp;
	int buttonState = 0;
	EventHandler handler;
	
};

