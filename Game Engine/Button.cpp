#include "Button.h"



Button::Button(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer, std::string buttonText, EventHandler buttonEventHandler)
	: GUIElement(x, y, width, height, visible, imageFilePath, targetSurface, targetRenderer)
{
	setButtonText(buttonText);
	handler = buttonEventHandler;
}

Button::~Button()
{
	delete rect;
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(textTexture);
}

int Button::getButtonImageWidth()
{
	return BUTTON_IMAGE_WIDTH;
}

int Button::getButtonImageHeight()
{
	return BUTTON_IMAGE_HEIGHT;
}

void Button::setButtonText(std::string newText)
{
	text = newText;
	SDL_DestroyTexture(textTexture);
	makeButtonTextTexture();
}

std::string Button::getButtonText()
{
	return text;
}

void Button::setIsMouseOver(bool mouseOver)
{
	isMouseOver = mouseOver;
	if (mouseOver)
	{
		buttonState = 1;
	}
	else
	{
		buttonState = 0;
	}
}

bool Button::getIsMouseOver()
{
	return isMouseOver;
}

void Button::setIsMouseDown(bool mouseDown)
{
	if (isMouseOver)
	{
		isMouseDown = mouseDown;
		isMouseUp = !mouseDown;
	}
	else
	{
		isMouseDown = false;
	}
	if (isMouseDown)
	{
		buttonState = 2;
	}
}

bool Button::getIsMouseDown()
{
	return isMouseDown;
}

void Button::setIsMouseUp(bool mouseUp)
{
	if (isMouseOver)
	{
		isMouseUp = mouseUp;
		isMouseDown = !mouseUp;
	}
	else
	{
		isMouseUp = false;
	}
	if (isMouseUp)
	{
		buttonState = 3;
	}
}

bool Button::getIsMouseUp()
{
	return isMouseUp;
}

int Button::getButtonState()
{
	return buttonState;
}

std::function<void()> Button::getEventHandler()
{
	return handler;
}

void Button::setEventHandler(EventHandler newHandler)
{
	handler = newHandler;
}

void Button::triggerEvent()
{
	if (getIsVisible() && handler != nullptr)
	{
		handler();
	}
}

bool Button::makeButtonTextTexture()
{
	bool success = true;
	if (text == "")
	{
		return success;
	}
	std::stringstream s;
	s << text;

	TTF_Font* font = TTF_OpenFont("segoeui.ttf", 24);
	if (!font)
	{
		printf("TTF_OpenFont Error: %s\n", TTF_GetError());
		return false;
	}
	SDL_Color textColor = { 0, 0, 0 };
	SDL_Surface* messageSurface;
	if (!(messageSurface = TTF_RenderText_Solid(font, s.str().c_str(), textColor)))
	{
		printf("Button text could not display. TTF Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		textTexture = SDL_CreateTextureFromSurface(trgtRenderer, messageSurface);
	}
	SDL_FreeSurface(messageSurface);
	TTF_CloseFont(font);
	return success;
}


bool Button::render()
{
	bool success = true;
	if (!isVisible)
	{
		return success;
	}
	else if (texture == nullptr)
	{
		printf("Unable to render Button: Missing texture!\n");
		success = false;
	}
	else
	{
		SDL_Rect spriteSheetClip = { buttonState * BUTTON_IMAGE_WIDTH,
			0,
			BUTTON_IMAGE_WIDTH, BUTTON_IMAGE_HEIGHT };
		SDL_RenderCopy(trgtRenderer, texture, &spriteSheetClip, rect);
		SDL_RenderCopy(trgtRenderer, textTexture, NULL, rect);
	}
	return success;
}
