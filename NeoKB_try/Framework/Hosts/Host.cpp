#include "Host.h"


#include "../Input/InputManager.h"
#include "../IO/Display.h"
#include "../Game.h"
#include "../../Instruments/Instrument.h"
#include "../IO/Storage.h"
#include <chrono>         // std::chrono::seconds
#include "../Threading/ThreadMaster.h"



using namespace std;
using namespace Framework::Hosts;
using namespace Framework::Input;
using namespace Framework::IO;
using namespace Framework;
using namespace Instruments;
using namespace Framework::Threading;




Host::Host(): RegisterType("Host")
{

}

int Host::Initialize(string name)
{
	LOG(LogLevel::Info) << "Host::Initialize() : Creating dependencies, storage and threads.";

	dependencies = new DependencyContainer();

	dependencies->Cache<Host>(this, "Host");
	LOG(LogLevel::Debug) << "Host::Initialize() : host address = " << this;

	// name�O��Ƨ��W��
	dependencies->Cache<Storage>(storage = getStorage(name), "Storage");


	setupMainInterface();


	drawThread = new GameThread(bind(&Host::drawFrame, this), "DrawThread");


	updateThread = new GameThread(bind(&Host::updateFrame, this), "UpdateThread");


	inputThread = new GameThread(bind(&Host::inputFrame, this), "InputThread");


	sceneGraphClock = updateThread->GetClock();

	initialized = true;

	LOG(LogLevel::Finest) << "Host::Initialize() : end.";

	

	return 0;
}

int Host::Run(Game* game, Instrument* instrument)
{
	LOG(LogLevel::Info) << "Host::Run() : Start host.";

	if(!initialized)
		throw runtime_error("int Host::Run() : Not initialized.");


	LOG(LogLevel::Finest) << "Host::Run() : Setup confing.";
	setupConfig();


	LOG(LogLevel::Finest) << "Host::Run() : reset input handler.";
	resetInputHandlers();

	LOG(LogLevel::Finest) << "Initialize Threads.";
	inputInitialize();
	inputThread->Start();

	drawInitialize();
	drawThread->Start();

	updateInitialize();
	updateThread->Start();

	ThreadMaster::GetInstance().SetFrequency(10);
	ThreadMaster::GetInstance().Start();

	if(!initialized)
		LOG(LogLevel::Finest) << "host not initialized yet.";

	bootstrapSceneGraph(game, instrument);
	
	LOG(LogLevel::Debug) << "Host::Run(): listing cache" << dependencies->_DebugPrintCache();

	
	runLoop();

	return 0;
}

MainInterface * Host::GetMainInterface()
{
	if (!initialized)
		throw runtime_error("int Host::GetMainInterface() : Not initialized.");

	return mainInterface;
}

vector<InputHandler*>* Host::GetAvailableInputHandlers()
{
	if (!initialized)
		throw runtime_error("int Host::GetAvailableInputHandlers() : Not initialized.");

	return availableInputHandlers;
}

Storage * Host::GetStorage()
{
	if (!initialized)
		throw runtime_error("int Host::GetStorage() : Not initialized.");

	return storage;
}

int Host::runLoop()
{
	// TODO: ���ίv�ɶ��B���_���X
	while(!exitRequested)
		this_thread::sleep_for(chrono::milliseconds(10));

	return 0;
}

int Host::drawInitialize()
{

	int width, height;
	frameworkConfigManager->Get(FrameworkSetting::Width, &width);
	frameworkConfigManager->Get(FrameworkSetting::Height, &height);

	LOG(LogLevel::Info) << "Host::drawInitialize() : Setting map(" << width << "," << height << ").";

	// �o�������\�bmain�̤~��A�o��S���s�ù��j�p
	canvas = new Map(width, height);

	drawThread->SetMaxUpdateHz(20);
	return 0;
}

int Host::drawFrame()
{
	if (root == nullptr)
		return 0;

	LOG(LogLevel::Depricated) << "Host::drawFrame() : drawing a frame.";

	canvas->Reset();

	// TODO: �j�M�Ҧ���child�A�ݬݬO���ODrawable�A�O���ܦA�ھ�depth�A�@�h�@�h�e��Map�W�A�M��call drawer
	vector<Drawable*> drawables;
	unique_lock<mutex> uLock(ChildMutex);
	//unique_lock<mutex> uLock2(TreeMutex2);	// ������n�Ψ��lock?
	iterateSearchDrawable(root, &drawables);
	uLock.unlock();
	//uLock2.unlock();

	LOG(LogLevel::Depricated) << "Host::drawFrame() : drawables size = [" << drawables.size() << "].";

	// TODO: �o�����ӭn��`�쪺drawable���Depth�ӱƧǡA���O�{�b�i�o�g

	for (int i = 0; i < drawables.size(); i++) {
		
		canvas->PasteAdd(drawables[i]->GetGraph(),
			drawables[i]->GetPositionX(), 
			drawables[i]->GetPositionY());
	}

	LOG(LogLevel::Depricated) << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "Host::drawFrame : light map";
		// �]���u�ݵe�������A�ҥH���ݨ�L��
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

	LOG(LogLevel::Depricated) << "Host::drawFrame() : pass to display.";

	mainInterface->GetDisplay()->Show(canvas);

	return 0;
}

int Host::updateInitialize()
{
	LOG(LogLevel::Info) << "Host::updateInitialize() : Setting update thread.";
	// �w�]�O1000
	updateThread->SetMaxUpdateHz(50);
	return 0;
}

int Host::updateFrame()
{
	if (root == nullptr)
		return 0;

	LOG(LogLevel::Depricated) << "Host::updateFrame() : update scenegraph tree.";

	LOG(LogLevel::Depricated) << "Host::updateFrame() : print tree " << [](Container* r) {

		r->_DebugPrintTree("|");

		return 0;
	}(root);

	root->UpdateSubTree();

	return 0;
}

int Host::inputInitialize()
{
	LOG(LogLevel::Info) << "Host::inputInitialize() : Setting input thread.";
	// �w�]�O1000�F
	inputThread->SetMaxUpdateHz(500);
	return 0;
}

int Host::inputFrame()
{
	LOG(LogLevel::Depricated) << "Host::inputFrame() : scan input.";

	mainInterface->ScanInput();
	/* �o��u��Xpanel�W��uotput�A����X�^��O�� */

	LOG(LogLevel::Depricated) << "Host::inputFrame() : process output.";
	mainInterface->ProcessOutput();

	LOG(LogLevel::Depricated) << "Host::inputFrame() : process output over.";

	return 0;
}

int Host::resetInputHandlers()
{
	LOG(LogLevel::Info) << "Host::resetInputHandlers() : create input handlers.";

	availableInputHandlers = createAvailableInputHandlers();

	LOG(LogLevel::Info) << "Host::resetInputHandlers() : input handlers initializing.";
	for (int i = 0; i < availableInputHandlers->size(); i++)
		availableInputHandlers->at(i)->Initialize(this);

	//availableInputHandlers.push_back(pianoKeyInputHandler);

	// �binitialize�̭��A�|��ۤv��on input���U��host.OnInput��
	// pianoKeyInputHandler->Intialize();

	return 0;
}

int Host::setupConfig()
{
	LOG(LogLevel::Info) << "Host::setupConfig() : loading default config.";

	frameworkConfigManager = new FrameworkConfigManager();
	frameworkConfigManager->Initialize(storage);

	//fConfigManager->Set(FrameworkSetting::SongTitle, string(argv[1]));  // �o�椧��n�R��
	frameworkConfigManager->Set(FrameworkSetting::PatternGenerator, string("MeteorPatternGenerator"));
	frameworkConfigManager->Set(FrameworkSetting::HardwareVersion, 10);
	frameworkConfigManager->Set(FrameworkSetting::Width, 48); //�n��
	frameworkConfigManager->Set(FrameworkSetting::Height, 16);
	frameworkConfigManager->Set(FrameworkSetting::BlackKeyHeight, 16);
	frameworkConfigManager->Set(FrameworkSetting::TargetHeight, 10);	// �C15	// ��10
	frameworkConfigManager->Set(FrameworkSetting::BlackKeyTargetHeight, 10);	// 14
	frameworkConfigManager->Set(FrameworkSetting::StartPitch, 24);
	frameworkConfigManager->Set(FrameworkSetting::FrameRate, 30);
	frameworkConfigManager->Set(FrameworkSetting::UpdateRate, 100);
	frameworkConfigManager->Set(FrameworkSetting::AudioUpdateRate, 1000);
	frameworkConfigManager->Set(FrameworkSetting::InputRate, 1000);
	frameworkConfigManager->Set(FrameworkSetting::SongTitle, string("OdeToJoy"));

	dependencies->Cache<FrameworkConfigManager>(frameworkConfigManager);
	

	

	return 0;
}

int Host::bootstrapSceneGraph(Game* game, Instrument* instrument)
{
	LOG(LogLevel::Info) << "Host::bootstrapSceneGraph() : build scenegraph tree.";
	LOG(LogLevel::Debug) << "Host::bootstrapSceneGraph() : host address = [" << this << "], clock = [" << sceneGraphClock << "].";

	root = game->CreateInputManager();

	dependencies->Cache(root);
	
	dependencies->Cache(game, "Game");
	game->SetHost(this);

	dependencies->Cache(instrument, "Instrument");
	instrument->SetHost(this);

	// root �nasync�A���M�|�ܦ��S��loaded
	root->LoadAsync(sceneGraphClock, dependencies);

	root->AddChild(game);

	InputManager* instrumentInputManager = instrument->CreateInputManager();
	root->AddChild(instrumentInputManager);
	instrumentInputManager->AddChild(instrument);


	return 0;
}

int Host::iterateSearchDrawable(ChildAddable * r, vector<Drawable*>* drawables)
{
	LOG(LogLevel::Depricated) << "Host::iterateSearchDrawable() : check is drawable [" << r << "].";

	if (dynamic_cast<Drawable*>(r)) 
		if(dynamic_cast<Drawable*>(r)->GetIsDrawable())
			drawables->push_back(dynamic_cast<Drawable*>(r));
	
	
	for (int i = 0; i < r->GetChilds()->size(); i++) {
		LOG(LogLevel::Depricated) << "Host::iterateSearchDrawable() : iterate search child [" << r->GetChilds()->at(i) << "].";
		iterateSearchDrawable(r->GetChilds()->at(i), drawables);
	}

	return 0;
}

