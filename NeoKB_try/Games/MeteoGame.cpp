#include "MeteoGame.h" 

#include "Scene\Loader.h"

using namespace Games;
using namespace Games::Scenes;


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
	AddChild(screenStack = new Loader());

	return 0;
}
