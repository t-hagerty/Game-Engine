#include "GameModel.h"
#include "GameView.h"
#include "GameController.h"

int main()
{
	GameModel* model = new GameModel();
	GameView* view = new GameView();
	GameController controller = GameController(model, view);
	return 0;
}
