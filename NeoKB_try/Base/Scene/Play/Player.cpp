#include "Player.h"

#include <string>
#include "../../Sheetmusic/Sheetmusic.h"

using namespace std;
using namespace Base::Scenes::Play;
using namespace Base::Rulesets;
using namespace Base::Sheetmusics;
using namespace Framework::Configurations;


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

	
		

	/***
	Sm<Event>* sm = workingSm->GetSm();
	 
	if (!rulesetInfo)
		rulesetInfo = sm->GetRulesetInfo();
	***/
	
	ruleset = rulesetInfo.GetValue()->CreateRuleset();

	rulesetExecutor = ruleset->CreateRulesetExecutor(workingSm.GetValue());

	AddChild(rulesetExecutor);

	return 0;
}

Player::Player(): RegisterType("Player"), MeteoScene()
{
	registerLoad(bind((int(Player::*)())&Player::load, this));
}

Player::~Player()
{
	delete ruleset;
	delete rulesetExecutor;
}
