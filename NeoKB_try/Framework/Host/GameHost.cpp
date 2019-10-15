#include "GameHost.h"


#include "../Input/UserInputManager.h"



using namespace std;
using namespace Framework::Host;
using namespace Framework::Input;



GameHost::GameHost(string name)
{
	dependencies = new DependencyContainer();

	setupMainInterface();

	drawThread = new GameThread(bind(&GameHost::drawFrame, this), "DrawThread");
	drawInitialize();

	updateThread = new GameThread(bind(&GameHost::updateFrame, this), "UpdateThread");
	updateInitialize();

	inputThread = new GameThread(bind(&GameHost::inputFrame, this), "InputThread");
	inputInitialize();

	sceneGraphClock = updateThread->GetClock();

}

int GameHost::Run(Game* game, Instrument* instrument)
{
	setupConfig();

	resetInputHandlers();


	inputThread->Start();
	drawThread->Start();
	updateThread->Start();
	bootstrapSceneGraph(game, instrument);


	return 0;
}

MainInterface * GameHost::GetMainInterface()
{
	return mainInterface;
}

vector<InputHandler*>* GameHost::GetAvailableInputHandlers()
{
	return availableInputHandlers;
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

	mainInterface->GetDisplay()->Show(canvas);

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
	/* 初始化input handlers */
	return 0;
}

int GameHost::inputFrame()
{

	mainInterface->ScanInput();
	/* 這邊只輸出panel上的uotput，不輸出琴鍵燈光 */
	mainInterface->ProcessOutput();
	return 0;
}

int GameHost::resetInputHandlers()
{
	availableInputHandlers = createAvailableInputHandlers();


	//availableInputHandlers.push_back(pianoKeyInputHandler);

	// 在initialize裡面，會把自己的on input註冊到host.OnInput裡
	// pianoKeyInputHandler->Intialize();

	return 0;
}

int GameHost::setupConfig()
{
	return 0;
}

int GameHost::bootstrapSceneGraph(Game* game, Instrument* instrument)
{
	root = new UserInputManager();

	dependencies->Cache(root);
	dependencies->Cache(game);
	dependencies->Cache(instrument);

	root->SetClock(sceneGraphClock);
	root->SetDependencies(dependencies);
	// root 要async，不然會變成沒有loaded
	root->Async();

	root->AddChild(game);
	game->SetHost(this);

	InputManager* instrumentInputManager = instrument->CreateInputManager();
	root->AddChild(instrumentInputManager);
	instrumentInputManager->AddChild(instrument);
	instrument->SetHost(this);


	return 0;
}

int GameHost::iterateSearchDrawable(ChildAddable * r, vector<Drawable*>* drawables)
{

	if (dynamic_cast<Drawable*>(r)) 
		if(r->GetIsDrawable())
			drawables->push_back(dynamic_cast<Drawable*>(r));
	
	
	for (int i = 0; i < r->GetChilds()->size(); i++) {
		iterateSearchDrawable(r->GetChilds()->at(i), drawables);
	}

	return 0;
}

