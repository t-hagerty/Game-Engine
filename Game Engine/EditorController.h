#pragma once
#include "EditorModel.h"
#include "EditorView.h"
#include <SDL.h>
class EditorController
{
public:
	EditorController(EditorModel* m, EditorView* v);
	~EditorController();
	SDL_Event e;
	int getExitCondition();
private:
	int fps = 0;
	void gameLoop();
	void update(double delta) const;
	void render() const;
	void mouseEventHandler(SDL_Event* e);
	void goToTest();
	void goToGame();
	void goToMenu();
	void setTileSelection(int selection);
	EditorModel* model;
	EditorView* view;
	//Key press surfaces constants
	enum KeyPresses
	{
		KEY_PRESS_DEFAULT,
		KEY_PRESS_UP,
		KEY_PRESS_DOWN,
		KEY_PRESS_LEFT,
		KEY_PRESS_RIGHT,
		KEY_PRESS_TOTAL
	};
	bool isUpAlreadyPressed = false;
	bool isLeftAlreadyPressed = false;
	bool isMouseBeingDragged = false;
	enum ExitConditions
	{
		QUIT,
		GAME,
		EDITOR,
		MAIN_MENU
	};
	int exitCondition = QUIT;
	bool quitLoop = false; //For terminating the game loop
};

