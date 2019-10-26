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
	// �o�䤣���D���˧�virtual function��bind�W�h�A�u�n��lambda��
	// smManager->GetStableStorage = bind(static_cast<Storage*(MeteoGame::*)(void)>(&MeteoGame::GetStableStorage), this);
	// ���ӬO���γo�ˡAvirtual function�|�۰�bind�W�hoverride�A���ίS�O�g��lambda��
	// https://stackoverflow.com/questions/14408105/how-do-i-use-stdbind-to-call-the-base-classs-version-of-a-virtual-function/14408253
	// �T�ꤣ�μglambda���A�ȱ�bind base::function�A�]�|bind override function�W�h
	smManager->GetStableStorage = [=]() {return GetStableStorage(); };

	AddChild(screenStack = new Loader());

	return 0;
}
