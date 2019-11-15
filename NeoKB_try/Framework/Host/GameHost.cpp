#include "GameHost.h"


#include "../Input/UserInputManager.h"
#include "../IO/Display.h"
#include "../Game.h"
#include "../../Instruments/Instrument.h"
#include "../IO/Storage.h"



using namespace std;
using namespace Framework::Host;
using namespace Framework::Input;
using namespace Framework::IO;
using namespace Framework;
using namespace Instruments;




GameHost::GameHost()
{

}

int GameHost::Initialize(string name)
{
	LOG(LogLevel::Info) << "GameHost::Initialize() : Creating dependencies, storage and threads.";

	dependencies = new DependencyContainer();

	// name是資料夾名稱
	dependencies->Cache<Storage>(storage = getStorage(name), "Storage");

	setupMainInterface();

	drawThread = new GameThread(bind(&GameHost::drawFrame, this), "DrawThread");


	updateThread = new GameThread(bind(&GameHost::updateFrame, this), "UpdateThread");


	inputThread = new GameThread(bind(&GameHost::inputFrame, this), "InputThread");


	sceneGraphClock = updateThread->GetClock();

	initialized = true;

	LOG(LogLevel::Finest) << "GameHost::Initialize() : end.";

	return 0;
}

int GameHost::Run(Game* game, Instrument* instrument)
{
	LOG(LogLevel::Info) << "GameHost::Run() : Start host.";

	if(!initialized)
		throw runtime_error("int GameHost::Run() : Not initialized.");

	setupConfig();

	resetInputHandlers();

	inputInitialize();
	inputThread->Start();

	drawInitialize();
	drawThread->Start();

	updateInitialize();
	updateThread->Start();

	bootstrapSceneGraph(game, instrument);


	return 0;
}

MainInterface * GameHost::GetMainInterface()
{
	if (!initialized)
		throw runtime_error("int GameHost::GetMainInterface() : Not initialized.");

	return mainInterface;
}

vector<InputHandler*>* GameHost::GetAvailableInputHandlers()
{
	if (!initialized)
		throw runtime_error("int GameHost::GetAvailableInputHandlers() : Not initialized.");

	return availableInputHandlers;
}

Storage * GameHost::GetStorage()
{
	if (!initialized)
		throw runtime_error("int GameHost::GetStorage() : Not initialized.");

	return storage;
}

int GameHost::drawInitialize()
{
	LOG(LogLevel::Info) << "GameHost::drawInitialize() : Setting draw thread.";

	int width, height;
	frameworkConfigManager->Get(FrameworkSetting::Width, &width);
	frameworkConfigManager->Get(FrameworkSetting::Height, &height);

	// 這個應該擺在main裡才對，這邊沒有存螢幕大小
	canvas = new Map(width, height);

	drawThread->SetMaxUpdateHz(50);
	return 0;
}

int GameHost::drawFrame()
{
	LOG(LogLevel::Finest) << "GameHost::drawFrame() : start drawing a frame.";

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
	LOG(LogLevel::Info) << "GameHost::updateInitialize() : Setting update thread.";
	// 預設是1000
	updateThread->SetMaxUpdateHz(500);
	return 0;
}

int GameHost::updateFrame()
{
	return root->UpdateSubTree();
}

int GameHost::inputInitialize()
{
	LOG(LogLevel::Info) << "GameHost::inputInitialize() : Setting input thread.";
	// 預設是1000了
	inputThread->SetMaxUpdateHz(500);;
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
	LOG(LogLevel::Info) << "GameHost::resetInputHandlers() : create input handlers.";

	availableInputHandlers = createAvailableInputHandlers();

	for (int i = 0; i < availableInputHandlers->size(); i++)
		availableInputHandlers->at(i)->Initialize(this);

	//availableInputHandlers.push_back(pianoKeyInputHandler);

	// 在initialize裡面，會把自己的on input註冊到host.OnInput裡
	// pianoKeyInputHandler->Intialize();

	return 0;
}

int GameHost::setupConfig()
{
	LOG(LogLevel::Info) << "GameHost::setupConfig() : loading default config.";

	frameworkConfigManager = new FrameworkConfigManager();
	//fConfigManager->Set(FrameworkSetting::SongTitle, string(argv[1]));  // 這行之後要刪掉
	frameworkConfigManager->Set(FrameworkSetting::PatternGenerator, string("MeteorPatternGenerator"));
	frameworkConfigManager->Set(FrameworkSetting::HardwareVersion, 10);
	frameworkConfigManager->Set(FrameworkSetting::Width, 48); //要改
	frameworkConfigManager->Set(FrameworkSetting::Height, 16);
	frameworkConfigManager->Set(FrameworkSetting::BlackKeyHeight, 16);
	frameworkConfigManager->Set(FrameworkSetting::TargetHeight, 15);	// 低20	// 高15
	frameworkConfigManager->Set(FrameworkSetting::BlackKeyTargetHeight, 10);	// 14
	frameworkConfigManager->Set(FrameworkSetting::StartPitch, 24);
	frameworkConfigManager->Set(FrameworkSetting::FrameRate, 30);
	dependencies->Cache<FrameworkConfigManager>(frameworkConfigManager);



	return 0;
}

int GameHost::bootstrapSceneGraph(Game* game, Instrument* instrument)
{
	root = new UserInputManager();

	dependencies->Cache(root);
	dependencies->Cache(game, "Game");
	dependencies->Cache(instrument, "Instrument");

	// root 要async，不然會變成沒有loaded
	root->LoadAsync(sceneGraphClock, dependencies);

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
		if(dynamic_cast<Drawable*>(r)->GetIsDrawable())
			drawables->push_back(dynamic_cast<Drawable*>(r));
	
	
	for (int i = 0; i < r->GetChilds()->size(); i++) {
		iterateSearchDrawable(r->GetChilds()->at(i), drawables);
	}

	return 0;
}

