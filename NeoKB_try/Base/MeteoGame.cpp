#include "MeteoGame.h" 

#include "Scene\Loader.h"

using namespace Base;
using namespace Base::Scenes;


int MeteoGame::load()
{
	return 0;
}

MeteoGame::MeteoGame() : RegisterType("MeteoGame"), MeteoGameBase()
{

	registerLoad(bind((int(MeteoGame::*)())&MeteoGame::load, this));

}

Bindable<RulesetInfo*>* MeteoGame::GetRuleset()
{
	return &ruleset;
}

int MeteoGame::LoadOnComplete()
{

	LoadAsync(screenStack = new Loader());

	return 0;
}
