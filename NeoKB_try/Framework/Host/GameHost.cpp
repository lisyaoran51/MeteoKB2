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
	// �o�������\�bmain�̤~��A�o��S���s�ù��j�p
	canvas = new Map(width, height);
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
	return 0;
}

int GameHost::updateFrame()
{
	return root->UpdateSubTree();
}

int GameHost::inputInitialize()
{
	/* ��l��input handlers */
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


	//availableInputHandlers.push_back(pianoKeyInputHandler);

	// �binitialize�̭��A�|��ۤv��on input���U��host.OnInput��
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
		if(r->GetIsDrawable())
			drawables->push_back(dynamic_cast<Drawable*>(r));
	
	
	for (int i = 0; i < r->GetChilds()->size(); i++) {
		iterateSearchDrawable(r->GetChilds()->at(i), drawables);
	}

	return 0;
}

