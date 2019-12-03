#include "GameHost.h"


#include "../Input/UserInputManager.h"
#include "../IO/Display.h"
#include "../Game.h"
#include "../../Instruments/Instrument.h"
#include "../IO/Storage.h"
#include <chrono>         // std::chrono::seconds
#include "../../RulesetMeteor/Config/MeteorConfigManager.h"



using namespace std;
using namespace Framework::Host;
using namespace Framework::Input;
using namespace Framework::IO;
using namespace Framework;
using namespace Instruments;
using namespace Meteor::Config;




GameHost::GameHost(): RegisterType("GameHost")
{

}

int GameHost::Initialize(string name)
{
	LOG(LogLevel::Info) << "GameHost::Initialize() : Creating dependencies, storage and threads.";

	dependencies = new DependencyContainer();

	dependencies->Cache<GameHost>(this, "GameHost");
	LOG(LogLevel::Debug) << "GameHost::Initialize() : host address = " << this;

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

	LOG(LogLevel::Finest) << "GameHost::Run() : Setup confing.";
	setupConfig();


	LOG(LogLevel::Finest) << "GameHost::Run() : reset input handler.";
	resetInputHandlers();

	LOG(LogLevel::Finest) << "Initialize Threads.";
	inputInitialize();
	//inputThread->Start();

	drawInitialize();
	drawThread->Start();

	updateInitialize();
	//updateThread->Start();

	if(!initialized)
		LOG(LogLevel::Finest) << "host not initialized yet.";


	bootstrapSceneGraph(game, instrument);
	
	LOG(LogLevel::Debug) << "GameHost::Run(): listing cache" << dependencies->_DebugPrintCache();

	
	runLoop();

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

int GameHost::runLoop()
{
	// TODO: 更改睡眠時間、中斷跳出
	while(!exitRequested)
		this_thread::sleep_for(chrono::milliseconds(10));

	return 0;
}

int GameHost::drawInitialize()
{

	int width, height;
	frameworkConfigManager->Get(FrameworkSetting::Width, &width);
	frameworkConfigManager->Get(FrameworkSetting::Height, &height);

	LOG(LogLevel::Info) << "GameHost::drawInitialize() : Setting map(" << width << "," << height << ").";

	// 這個應該擺在main裡才對，這邊沒有存螢幕大小
	canvas = new Map(width, height);

	drawThread->SetMaxUpdateHz(1);
	return 0;
}

int GameHost::drawFrame()
{
	LOG(LogLevel::Finest) << "GameHost::drawFrame() : drawing a frame.";

	canvas->Reset();

	// TODO: 搜尋所有的child，看看是不是Drawable，是的話再根據depth，一層一層畫到Map上，然後call drawer
	vector<Drawable*> drawables;
	unique_lock<mutex> uLock(ChildMutex);
	iterateSearchDrawable(root, &drawables);
	uLock.unlock();

	LOG(LogLevel::Finer) << "GameHost::drawFrame() : drawables size = [" << drawables.size() << "].";

	// TODO: 這邊應該要把蒐到的drawable跟具Depth來排序，但是現在懶得寫

	for (int i = 0; i < drawables.size(); i++) {
		
		canvas->PasteAdd(drawables[i]->GetGraph(),
			drawables[i]->GetPositionX(), 
			drawables[i]->GetPositionY());
	}

	LOG(LogLevel::Depricated) << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "GameHost::drawFrame : light map";
		// 因為只看畫面中央，所以不看其他排
		for (int i = 0; i < width; i++) {
			string s;
			for (int j = 0; j < height; j++) {
				s += to_string(m->Get(i, j));
				s += " ";
			}
			LOG(LogLevel::Finest) << "| " << s << "|";
		}
		return 0;
	}(canvas->GetWidth(), canvas->GetHeight(), canvas);

	LOG(LogLevel::Finest) << "GameHost::drawFrame() : pass to display.";

	mainInterface->GetDisplay()->Show(canvas);

	return 0;
}

int GameHost::updateInitialize()
{
	LOG(LogLevel::Info) << "GameHost::updateInitialize() : Setting update thread.";
	// 預設是1000
	updateThread->SetMaxUpdateHz(1);
	return 0;
}

int GameHost::updateFrame()
{
	LOG(LogLevel::Finest) << "GameHost::updateFrame() : update scenegraph tree.";

	LOG(LogLevel::Finest) << "GameHost::updateFrame() : print tree " << [](Container* r) {

		r->_DebugPrintTree("|");

		return 0;
	}(root);

	return root->UpdateSubTree();
}

int GameHost::inputInitialize()
{
	LOG(LogLevel::Info) << "GameHost::inputInitialize() : Setting input thread.";
	// 預設是1000了
	inputThread->SetMaxUpdateHz(1);;
	return 0;
}

int GameHost::inputFrame()
{
	LOG(LogLevel::Finest) << "GameHost::inputFrame() : scan input.";

	mainInterface->ScanInput();
	/* 這邊只輸出panel上的uotput，不輸出琴鍵燈光 */

	LOG(LogLevel::Finest) << "GameHost::inputFrame() : process output.";
	mainInterface->ProcessOutput();


	return 0;
}

int GameHost::resetInputHandlers()
{
	LOG(LogLevel::Info) << "GameHost::resetInputHandlers() : create input handlers.";

	availableInputHandlers = createAvailableInputHandlers();

	LOG(LogLevel::Info) << "GameHost::resetInputHandlers() : input handlers initializing.";
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
	frameworkConfigManager->Initialize();

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
	

	MeteorConfigManager* meteorConfigManager = new MeteorConfigManager();
	meteorConfigManager->Initialize();

	meteorConfigManager->Set(MeteorSetting::WhiteKeyTargetHeight, 9);	// 低4	  高9
	meteorConfigManager->Set(MeteorSetting::BlackKeyTargetHeight, 8);
	meteorConfigManager->Set(MeteorSetting::FallSpeed, MTO_FLOAT(8.f));	//正常16.f	慢12.f
	meteorConfigManager->Set(MeteorSetting::FallBrightness, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::FallLength, 1);
	meteorConfigManager->Set(MeteorSetting::ExplodeSpeed, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::ExplodeBrightness, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::GlowLineSpeed, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::GlowLineDuration, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::GlowLineBrightness, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::TargetLineBlinkSpeed, MTO_FLOAT(0.75f));
	meteorConfigManager->Set(MeteorSetting::TargetLineBrightness, MTO_FLOAT(0.05f));
	dependencies->Cache<MeteorConfigManager>(meteorConfigManager);

	return 0;
}

int GameHost::bootstrapSceneGraph(Game* game, Instrument* instrument)
{
	LOG(LogLevel::Info) << "GameHost::bootstrapSceneGraph() : build scenegraph tree.";
	LOG(LogLevel::Debug) << "GameHost::bootstrapSceneGraph() : host address = [" << this << "], clock = [" << sceneGraphClock << "].";

	root = new UserInputManager();

	dependencies->Cache(root);
	
	dependencies->Cache(game, "Game");
	game->SetHost(this);

	dependencies->Cache(instrument, "Instrument");
	instrument->SetHost(this);

	// root 要async，不然會變成沒有loaded
	root->LoadAsync(sceneGraphClock, dependencies);

	root->AddChild(game);

	InputManager* instrumentInputManager = instrument->CreateInputManager();
	root->AddChild(instrumentInputManager);
	instrumentInputManager->AddChild(instrument);


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

