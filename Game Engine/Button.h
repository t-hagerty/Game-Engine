#include "GUIElement.h"
#include <SDL.h>
#include <string>
#include <functional>
#pragma once
class Button :
	public GUIElement
{
public:
	typedef std::function<void(int)> EventHandler;
	Button(double x, double y, double width, double height, bool visible, std::string imageFilePath = "default_button.bmp", SDL_Surface* targetSurface = nullptr, SDL_Renderer* targetRenderer = nullptr, std::string buttonText = "", EventHandler buttonEventHandler = NULL);
	~Button();
	int getButtonImageWidth();
	int getButtonImageHeight();
	void setButtonText(std::string newText);
	std::string getButtonText();
	void setIsMouseOver(bool mouseOver);
	bool getIsMouseOver();
	void setIsMouseDown(bool mouseDown);
	bool getIsMouseDown();
	void setIsMouseUp(bool mouseUp);
	bool getIsMouseUp();
	void setButtonState(int newState);
	int getButtonState();
	void setTexture(SDL_Texture* newTexture) override;
	void setIsHighlighted(bool highlighted);
	bool getIsHighlighted();
	EventHandler getEventHandler();
	void setEventHandler(EventHandler newHandler);
	void triggerEvent();
	void setEventArg(int newArg);
	int getEventArg();
	bool render() override;
private:
	const int BUTTON_IMAGE_WIDTH = 32;
	const int BUTTON_IMAGE_HEIGHT = 32;
	int numberFrames;
	std::string text;
	SDL_Texture* textTexture;
	bool isMouseOver;
	bool isMouseDown;
	bool isMouseUp;
	bool isHighlighted = false;
	int buttonState = 0; //spritesheet 0 = default, 1 = mouseover, 2 = mousedown, 3 = mouseup
	EventHandler handler;
	int eventArgument = -1;
	bool makeButtonTextTexture();
};

