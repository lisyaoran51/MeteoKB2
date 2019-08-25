#include "MtoGame.h" 

#include "Scene\Loader.h"

using namespace Base;
using namespace Base::Scenes;


int MtoGame::load()
{
	return 0;
}

MtoGame::MtoGame() : RegisterType("MtoGame"), MtoGameBase()
{

	registerLoad(bind((int(MtoGame::*)())&MtoGame::load, this));

}

int MtoGame::LoadOnComplete()
{

	LoadAsync(screenStack = new Loader());

	return 0;
}
