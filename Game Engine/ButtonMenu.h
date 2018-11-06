#pragma once
#include "GUIElement.h"
#include "Button.h"
#include <vector>
class ButtonMenu :
	public GUIElement
{
public:
	typedef std::function<void()> EventHandler;
	ButtonMenu(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer,
		int maxBttnHght, int wMargin, int hMargin, int numCols);
	ButtonMenu(double x, double y, double width, double height, bool visible, std::string imageFilePath, SDL_Surface* targetSurface, SDL_Renderer* targetRenderer,
		int maxBttnHght, int wMargin, int hMargin, int numCols, std::vector<Button*> buttonsInMenu);
	~ButtonMenu();
	int getMaxButtonHeight();
	void setMaxButtonHeight(int newMaxHeight);
	int getWidthMargins();
	void setWidthMargins(int newMargin);
	int getHeightMargins();
	void setHeightMargins(int newMargin);
	std::vector<Button*> getButtons();
	void setButtons(std::vector<Button*> newButtons);
	void addButton(Button* aButton);
	void addButton(std::string imageFilePath, std::string buttonText, EventHandler buttonEventHandler);
	void removeButton(Button* aButton);
	int getNumberButtons();
	void setIsVisible(bool visible) override;
	void toggleVisibility() override;
	bool render() override;
private:
	int maxButtonHeight;
	int widthMargins;
	int heightMargins;
	int buttonColumns;
	std::vector<Button*> buttons;
};

