#include "MeteoGame.h" 

#include "Scene\Loader.h"

using namespace Games;
using namespace Games::Scenes;


int MeteoGame::load()
{
	GetDependencies()->Cache<MeteoGame>(this, "MeteoGame");
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

Storage * MeteoGame::GetStableStorage()
{
	return nullptr;
}

int MeteoGame::LoadOnComplete()
{
	// 這邊不知道怎麼樣把virtual function給bind上去，只好用lambda式
	// smManager->GetStableStorage = bind(static_cast<Storage*(MeteoGame::*)(void)>(&MeteoGame::GetStableStorage), this);
	// 應該是不用這樣，virtual function會自動bind上去override，不用特別寫成lambda式
	// https://stackoverflow.com/questions/14408105/how-do-i-use-stdbind-to-call-the-base-classs-version-of-a-virtual-function/14408253
	// 確實不用寫lambda式，值接bind base::function，也會bind override function上去
	smManager->GetStableStorage = [=]() {return GetStableStorage(); };

	AddChild(screenStack = new Loader());

	return 0;
}
