#include "GameHost.h"


#include "../Input/InputManager.h"

using namespace std;
using namespace Framework::Host;
using namespace Framework::Input;

GameHost::GameHost(string name = "")
{
	drawThread = new GameThread(bind(&GameHost::drawFrame, this), "DrawThread");
	drawInitialize();

	updateThread = new GameThread(bind(&GameHost::updateFrame, this), "UpdateThread");
	updateInitialize();

	inputThread = new GameThread(bind(&GameHost::inputFrame, this), "InputThread");
	inputInitialize();

	sceneGraphClock = updateThread->GetClock();
}

int GameHost::drawInitialize()
{
	// 這個應該擺在main裡才對，這邊沒有存螢幕大小
	canvas = new Map(width, height);
	return 0;
}

int GameHost::drawFrame()
{
	canvas->Reset();

	// TODO: 搜尋所有的child，看看是不是Drawable，是的話再根據depth，一層一層畫到Map上，然後call drawer
	vector<Drawable*> drawables;
	iterateSearchDrawable(root, &drawables);

	// TODO: 這邊應該要把蒐到的drawable跟具Depth來排序，但是現在懶得寫

	for (int i = 0; i < drawables.size(); i++) {
		canvas->PasteAdd(drawables[i]->GetGraph(),
			drawables[i]->GetPositionX(), 
			drawables[i]->GetPositionY());
	}

	mainInterface.Display->Display(canvas);

	return 0;
}

int GameHost::updateInitialize()
{
	return 0;
}

int GameHost::updateFrame()
{
	return root->UpdateSubTree();
}

int GameHost::inputInitialize()
{
	return 0;
}

int GameHost::inputFrame()
{

	return 0;
}

int GameHost::resetInputHandler()
{

	availableInputHandler.push_back(pianoKeyInputHandler);

	// 在initialize裡面，會把自己的on input註冊到host.OnInput裡
	pianoKeyInputHandler->Intialize();

	return 0;
}

int GameHost::bootstrapSceneGraph(Game game)
{
	root = new InputManager();

	root->SetClock(sceneGraphClock);

	game->SetHost(this);



	return 0;
}

int GameHost::iterateSearchDrawable(ChildAddable * r, vector<Drawable*>* drawables)
{

	if (dynamic_cast<Drawable*>(r)) {
		drawables->push_back(dynamic_cast<Drawable*>(r));
	}
	
	for (int i = 0; i < r->GetChilds()->size(); i++) {
		iterateSearchDrawable(r->GetChilds()->at(i), drawables);
	}

	return 0;
}

