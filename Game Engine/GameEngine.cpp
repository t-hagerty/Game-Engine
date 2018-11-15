#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"
#include "EditorView.h"
#include "EditorModel.h"
#include "EditorController.h"


int main()
{
	/*GameModel* model = new GameModel();
	GameView* view = new GameView(model->getLevelWidth(), model->getLevelHeight());
	GameController controller = GameController(model, view);*/
	bool quit = false;
	enum ExitConditions
	{
		QUIT,
		GAME,
		EDITOR,
		MAIN_MENU
	};
	int router = GAME;
	GameModel* aGameModel;
	GameView* aGameView;
	GameController* aGameController;
	EditorModel* anEditorModel;
	EditorView* anEditorView;
	EditorController* anEditorController;
	while (!quit)
	{
		switch (router)
		{
		case QUIT:
			quit = true;
			break;
		case GAME:
			aGameModel =  new GameModel();
			aGameView = new GameView(aGameModel->getLevelWidth(), aGameModel->getLevelHeight());
			aGameController = new GameController(aGameModel, aGameView);
			router = aGameController->getExitCondition();
			delete aGameModel;
			delete aGameView;
			delete aGameController;
			break;
		case EDITOR:
			anEditorModel = new EditorModel();
			anEditorView = new EditorView(anEditorModel->getLevelWidth(), anEditorModel->getLevelHeight());
			anEditorController = new EditorController(anEditorModel, anEditorView);
			router = anEditorController->getExitCondition();
			delete anEditorModel;
			delete anEditorView;
			delete anEditorController;
			break;
		case MAIN_MENU:
			break;
		default:
			printf("ERROR: Invalid program routing state %d", router);
			quit = true;
			break;
		}
	}
	return 0;
}
