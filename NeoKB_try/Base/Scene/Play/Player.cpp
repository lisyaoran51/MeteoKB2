#include "Player.h"

#include <string>
#include "Session.h"
#include "../Sheetmusic/Sheetmusic.h"

using namespace std;
using namespace Base::Play;
using namespace Util::Hierachal;
using namespace Base::Rulesets;
using namespace Base::Sheetmusics;
using namespace Base::Config;


int Player::load()
{
	FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int  RulesetExecutor<T>::load() : FrameworkConfigManager not found in cache.");

	return load(f);
}

int Player::load(FrameworkConfigManager* f)
{
	LOG(LogLevel::Info) << "Player::load : start loading the player and reading the sm and ruleset from session.";

	Session* s = GetCache<Session>("Session");

	rulesetInfo = s->GetRulesetInfo();
	
		
	string songTitle;
	
	if (f->Get(FrameworkSetting::SongTitle, &songTitle))
		workingSm = s->GetWorkingSm(songTitle);			// workingSm要在遊戲結束以後刪掉
	else
		workingSm = s->GetWorkingSm();					// 這個寫法之後應該要改掉
	
	LOG(LogLevel::Finer) << "Player::load : get working sm [" << (int)workingSm << "]";

	/***
	Sm<Event>* sm = workingSm->GetSm();
	 
	if (!rulesetInfo)
		rulesetInfo = sm->GetRulesetInfo();
	***/
	
	ruleset = rulesetInfo->CreateRuleset();

	rulesetExecutor = ruleset->CreateRulesetExecutor(workingSm);

	AddChild(rulesetExecutor);

	return 0;
}

Player::Player(): RegisterType("Player"), ChildAddable()
{
	registerLoad(bind((int(Player::*)())&Player::load, this));
}

Base::Play::Player::~Player()
{
	delete ruleset;
	delete rulesetExecutor;
	delete workingSm;
}
