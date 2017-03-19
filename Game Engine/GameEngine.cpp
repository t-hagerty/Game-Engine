#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"

int main()
{
	GameModel* model = new GameModel(600, 400);
	GameView* view = new GameView();
	GameController controller = GameController(model, view);
	return 0;
}
