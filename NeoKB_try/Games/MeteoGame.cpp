#include "MeteoGame.h" 

#include "Scene/Loader.h"

using namespace Games;
using namespace Games::Scenes;


int MeteoGame::load()
{

	LOG(LogLevel::Info) << "MeteoGame::load() : caching itself.";

	GetDependencies()->Cache<MeteoGame>(this, "MeteoGame");

	return 0;
}

Intro * Games::MeteoGame::getIntro()
{
	
	for (Scene* s = screenStack; s != nullptr; s = s->GetChildScene()) {
		if (dynamic_cast<Intro*>(s) != nullptr) {
			intro = dynamic_cast<Intro*>(s);
			return dynamic_cast<Intro*>(s);

		}
		
	}

	return nullptr;
}

MeteoGame::MeteoGame() : RegisterType("MeteoGame")
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
	LOG(LogLevel::Info) << "MeteoGame::LoadOnComplete() : add loader into screen stack.";

	// 這邊不知道怎麼樣把virtual function給bind上去，只好用lambda式
	// smManager->GetStableStorage = bind(static_cast<Storage*(MeteoGame::*)(void)>(&MeteoGame::GetStableStorage), this);
	// 應該是不用這樣，virtual function會自動bind上去override，不用特別寫成lambda式
	// https://stackoverflow.com/questions/14408105/how-do-i-use-stdbind-to-call-the-base-classs-version-of-a-virtual-function/14408253
	// 確實不用寫lambda式，值接bind base::function，也會bind override function上去
	smManager->GetStableStorage = [=]() {return GetStableStorage(); };

	AddChild(screenStack = new Loader());

	return 0;
}
