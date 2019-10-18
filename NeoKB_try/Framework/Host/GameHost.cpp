#include "GameHost.h"


#include "../Input/UserInputManager.h"
#include "../IO/Display.h"
#include "../Game.h"
#include "../../Instruments/Instrument.h"



using namespace std;
using namespace Framework::Host;
using namespace Framework::Input;
using namespace Framework::IO;
using namespace Framework;
using namespace Instruments;




GameHost::GameHost(string name)
{
	dependencies = new DependencyContainer();

	setupMainInterface();

	drawThread = new GameThread(bind(&GameHost::drawFrame, this), "DrawThread");
	

	updateThread = new GameThread(bind(&GameHost::updateFrame, this), "UpdateThread");
	

	inputThread = new GameThread(bind(&GameHost::inputFrame, this), "InputThread");
	

	sceneGraphClock = updateThread->GetClock();

}

int GameHost::Run(Game* game, Instrument* instrument)
{
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
	return mainInterface;
}

vector<InputHandler*>* GameHost::GetAvailableInputHandlers()
{
	return availableInputHandlers;
}

int GameHost::drawInitialize()
{
	int width, height;
	frameworkConfigManager->Get(FrameworkSetting::Width, &width);
	frameworkConfigManager->Get(FrameworkSetting::Height, &height);

	// �o�������\�bmain�̤~��A�o��S���s�ù��j�p
	canvas = new Map(width, height);

	drawThread->SetMaxUpdateHz(50);
	return 0;
}

int GameHost::drawFrame()
{
	canvas->Reset();

	// TODO: �j�M�Ҧ���child�A�ݬݬO���ODrawable�A�O���ܦA�ھ�depth�A�@�h�@�h�e��Map�W�A�M��call drawer
	vector<Drawable*> drawables;
	iterateSearchDrawable(root, &drawables);

	// TODO: �o�����ӭn��`�쪺drawable���Depth�ӱƧǡA���O�{�b�i�o�g

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
	// �w�]�O1000
	updateThread->SetMaxUpdateHz(500);
	return 0;
}

int GameHost::updateFrame()
{
	return root->UpdateSubTree();
}

int GameHost::inputInitialize()
{
	// �w�]�O1000�F
	inputThread->SetMaxUpdateHz(500);;
	return 0;
}

int GameHost::inputFrame()
{

	mainInterface->ScanInput();
	/* �o��u��Xpanel�W��uotput�A����X�^��O�� */
	mainInterface->ProcessOutput();
	return 0;
}

int GameHost::resetInputHandlers()
{
	availableInputHandlers = createAvailableInputHandlers();

	for (int i = 0; i < availableInputHandlers->size(); i++)
		availableInputHandlers->at(i)->Initialize(this);

	//availableInputHandlers.push_back(pianoKeyInputHandler);

	// �binitialize�̭��A�|��ۤv��on input���U��host.OnInput��
	// pianoKeyInputHandler->Intialize();

	return 0;
}

int GameHost::setupConfig()
{
	frameworkConfigManager = new FrameworkConfigManager();
	//fConfigManager->Set(FrameworkSetting::SongTitle, string(argv[1]));  // �o�椧��n�R��
	frameworkConfigManager->Set(FrameworkSetting::PatternGenerator, string("MeteorPatternGenerator"));
	frameworkConfigManager->Set(FrameworkSetting::HardwareVersion, 10);
	frameworkConfigManager->Set(FrameworkSetting::Width, 48); //�n��
	frameworkConfigManager->Set(FrameworkSetting::Height, 16);
	frameworkConfigManager->Set(FrameworkSetting::BlackKeyHeight, 16);
	frameworkConfigManager->Set(FrameworkSetting::TargetHeight, 15);	// �C20	// ��15
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

	root->SetClock(sceneGraphClock);
	root->SetDependencies(dependencies);
	// root �nasync�A���M�|�ܦ��S��loaded
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
		if(dynamic_cast<Drawable*>(r)->GetIsDrawable())
			drawables->push_back(dynamic_cast<Drawable*>(r));
	
	
	for (int i = 0; i < r->GetChilds()->size(); i++) {
		iterateSearchDrawable(r->GetChilds()->at(i), drawables);
	}

	return 0;
}

