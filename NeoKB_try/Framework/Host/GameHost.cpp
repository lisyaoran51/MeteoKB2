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
	return 0;
}

int GameHost::drawFrame()
{
	// TODO: 搜尋所有的child，看看是不是Drawable，是的話再根據depth，一層一層畫到Map上，然後call drawer

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

