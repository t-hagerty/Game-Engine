#pragma once
#include "View.h"

class EditorView :
	public View
{
public:
	EditorView(int levelW, int levelH, int windowW, int windowH, SDL_Window* window, SDL_Surface* screen, SDL_Renderer* renderer);
	~EditorView();
	std::tuple<int, int> convertScreenCoordsToModel(int mouseX, int mouseY);
	void renderTileMap(std::vector<Tile*> map, int rows, int cols, int tileSize, int frame) override;
	void renderEntitySprite(Entity* e, int frame) override;
	void moveCameraLeft();
	void moveCameraRight();
	void moveCameraUp();
	void moveCameraDown();
	void setButtonHandlers(EventHandler retryButtonHandler, EventHandler gameButtonHandler, EventHandler mainMenuButtonHandler);
	void setSelectionButtonHandlers(EventHandler handler);
private:
	std::vector<Tile*> tileAnimationSynchronizer;
	Button* gameButton;
	Button* testButton;
	Button* retryButton;
	ButtonMenu* selectionMenu;
	int selectionMenuWidth = 126;

	virtual bool initGUI() override;
	void populateSelectionMenu();
	int getAnimationFrameForTile(int type);
	void incrementTileAnimationSynchronizer();
	void moveCamera(int xChange, int yChange);
};

