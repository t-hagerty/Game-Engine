#include "Button.h"



Button::Button(double x, double y, double width, double height, std::string imageFilePath, std::string buttonText, EventHandler buttonEventHandler)
{
	buttonRect = new SDL_Rect();
	buttonRect->x = x;
	buttonRect->y = y;
	buttonRect->w = width;
	buttonRect->h = height;
	imageFile = imageFilePath;
	text = buttonText;
	isVisible = true;
	handler = buttonEventHandler;
}

Button::~Button()
{
}

void Button::setPosX(double newPos)
{
	buttonRect->x = newPos;
}

double Button::getPosX()
{
	return buttonRect->x;
}

void Button::setPosY(double newPos)
{
	buttonRect->y = newPos;
}

double Button::getPosY()
{
	return buttonRect->y;
}

void Button::setWidth(double newWidth)
{
	buttonRect->w = newWidth;
}

double Button::getWidth()
{
	return buttonRect->w;
}

void Button::setHeight(double newHeight)
{
	buttonRect->h = newHeight;
}

double Button::getHeight()
{
	return buttonRect->h;
}

void Button::setImageFilePath(std::string newFilePath)
{
	imageFile = newFilePath;
}

std::string Button::getImageFilePath()
{
	return imageFile;
}

void Button::setButtonTexture(SDL_Texture * newTexture)
{
	buttonTexture = newTexture;
}

SDL_Texture * Button::getButtonTexture()
{
	return buttonTexture;
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
}

std::string Button::getButtonText()
{
	return text;
}

void Button::setButtonrect(SDL_Rect * newRect)
{
	delete buttonRect;
	buttonRect = newRect;
}

SDL_Rect * Button::getButtonRect()
{
	return buttonRect;
}

void Button::setIsVisible(bool visible)
{
	isVisible = visible;
}

bool Button::getIsVisible()
{
	return isVisible;
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

void Button::triggerEvent()
{
	handler();
}
