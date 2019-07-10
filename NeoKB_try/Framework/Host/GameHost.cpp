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

	// �binitialize�̭��A�|��ۤv��on input���U��host.OnInput��
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

