#include "ButtonMenu.h"

ButtonMenu::ButtonMenu(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface * targetSurface, SDL_Renderer * targetRenderer,
	int maxBttnHght, int wMargin, int hMargin, int numCols) : GUIElement(x, y, width, height, visible, imageFilePath, targetSurface, targetRenderer)
{
	maxButtonHeight = maxBttnHght;
	widthMargins = wMargin;
	heightMargins = hMargin;
	buttonColumns = numCols;
}

ButtonMenu::ButtonMenu(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface * targetSurface, SDL_Renderer * targetRenderer,
	int maxBttnHght, int wMargin, int hMargin, int numCols, std::vector<Button*> buttonsInMenu) : GUIElement(x, y, width, height, visible, imageFilePath, targetSurface, targetRenderer)
{
	maxButtonHeight = maxBttnHght;
	widthMargins = wMargin;
	heightMargins = hMargin;
	buttonColumns = numCols;
	buttons = buttonsInMenu;
}

ButtonMenu::~ButtonMenu()
{
}

int ButtonMenu::getMaxButtonHeight()
{
	return maxButtonHeight;
}

void ButtonMenu::setMaxButtonHeight(int newMaxHeight)
{
	maxButtonHeight = newMaxHeight;
}

int ButtonMenu::getWidthMargins()
{
	return widthMargins;
}

void ButtonMenu::setWidthMargins(int newMargin)
{
	widthMargins = newMargin;
}

int ButtonMenu::getHeightMargins()
{
	return heightMargins;
}

void ButtonMenu::setHeightMargins(int newMargin)
{
	heightMargins = newMargin;
}

std::vector<Button*> ButtonMenu::getButtons()
{
	return buttons;
}

void ButtonMenu::setButtons(std::vector<Button*> newButtons)
{
	buttons = newButtons;
}

void ButtonMenu::addButton(Button * aButton)
{
	int buttonHeight = maxButtonHeight;
	int buttonWidth = (getWidth() - ((buttonColumns + 1) * widthMargins)) / buttonColumns;
	buttons.insert(buttons.end(), aButton);
	aButton->setWidth(buttonWidth);
	if (((getNumberButtons() / buttonColumns) + heightMargins) * maxButtonHeight + heightMargins > getHeight())
	{
		buttonHeight = ((getHeight() - heightMargins) / (getNumberButtons() / buttonColumns)) - heightMargins;
		for (Button* b : buttons)
		{
			b->setHeight(buttonHeight);
		}
	}
	else
	{
		aButton->setHeight(buttonHeight);
	}
	int posXofFirstButton = getPosX() + widthMargins;
	int posYofFirstButton = getPosY() + heightMargins;
	buttons[0]->setPosX(posXofFirstButton);
	buttons[0]->setPosY(posYofFirstButton);
	for (std::size_t i = 1; i < buttons.size(); i++)
	{
		buttons[i]->setPosX(((i % buttonColumns) * (buttonWidth + widthMargins)) + posXofFirstButton);
		buttons[i]->setPosY(((i / buttonColumns) * (buttonHeight + heightMargins)) + posYofFirstButton);
	}
}

void ButtonMenu::addButton(std::string imageFilePath, std::string buttonText, EventHandler buttonEventHandler)
{
	Button* aButton = new Button(0, 0, 0, 0, false, imageFilePath, trgtSurface, trgtRenderer, buttonText, buttonEventHandler);
	addButton(aButton);
}

void ButtonMenu::removeButton(Button * aButton)
{
	for (int i = 0; i < static_cast<int>(buttons.size()); i++)
	{
		if (buttons[i] == aButton)
		{
			buttons.erase(buttons.begin() + i);
			return;
		}
	}
}

int ButtonMenu::getNumberButtons()
{
	return buttons.size();
}

void ButtonMenu::setIsVisible(bool visible)
{
	for (Button* b : buttons)
	{
		b->setIsVisible(visible);
	}
	GUIElement::setIsVisible(visible);
}

void ButtonMenu::toggleVisibility()
{
	GUIElement::toggleVisibility();
	for (Button* b : buttons)
	{
		b->setIsVisible(isVisible);
	}
}

bool ButtonMenu::render()
{
	bool success = true;
	if (!GUIElement::render())
	{
		success = false;
		printf("ButtonMenu background could not render!\n");
	}
	for (std::size_t i = 0; i < buttons.size(); i++)
	{
		if (!buttons[i]->render())
		{
			success = false;
			printf("Button #%d in menu could not render!\n", i);
		}
	}
	return success;
}
