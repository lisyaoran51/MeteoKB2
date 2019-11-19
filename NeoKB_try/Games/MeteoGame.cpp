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

	// �o�䤣���D���˧�virtual function��bind�W�h�A�u�n��lambda��
	// smManager->GetStableStorage = bind(static_cast<Storage*(MeteoGame::*)(void)>(&MeteoGame::GetStableStorage), this);
	// ���ӬO���γo�ˡAvirtual function�|�۰�bind�W�hoverride�A���ίS�O�g��lambda��
	// https://stackoverflow.com/questions/14408105/how-do-i-use-stdbind-to-call-the-base-classs-version-of-a-virtual-function/14408253
	// �T�ꤣ�μglambda���A�ȱ�bind base::function�A�]�|bind override function�W�h
	smManager->GetStableStorage = [=]() {return GetStableStorage(); };

	AddChild(screenStack = new Loader());

	return 0;
}
