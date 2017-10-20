#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"


int main()
{
	GameModel* model = new GameModel();
	GameView* view = new GameView(model->getLevelWidth(), model->getLevelHeight());
	GameController controller = GameController(model, view);
	return 0;
}
