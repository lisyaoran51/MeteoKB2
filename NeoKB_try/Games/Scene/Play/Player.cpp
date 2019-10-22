#include "Player.h"

#include <string>
#include "../../Sheetmusic/Sheetmusic.h"

using namespace std;
using namespace Games::Scenes::Play;
using namespace Games::Rulesets;
using namespace Games::Sheetmusics;
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

	WorkingSm* workingSmValue = workingSm.GetValue();

	ruleset = rulesetInfo.GetValue()->CreateRuleset();

	rulesetExecutor = ruleset->CreateRulesetExecutor(workingSm.GetValue());

	/***
	Sm<Event>* sm = workingSm->GetSm();
	 
	if (!rulesetInfo)
		rulesetInfo = sm->GetRulesetInfo();
	***/
	if (workingSmValue->GetTrack() == nullptr)
		adjustableClock = new StopwatchClock();
	else
		adjustableClock = workingSmValue->GetTrack();

	// 這個是當track停止以後，還可以繼續跑的鍾
	decoupledClock = new DecoupledInterpolatingFramedClock();
	decoupledClock->SetIsCoupled(false);

	offsetClock = new OffsetFramedClock(decoupledClock);

	// TODO: 把config裡面的offset和offset clock的offset bind在一起，讓config可以調整offset

	GetScheduler()->AddTask([=]() {
		decoupledClock->Reset();

		// TODO: 把mod加入adjustableClock裡
		// foreach (var mod in working.Mods.Value.OfType<IApplicableToClock>())
		//     mod.ApplyToClock(adjustableSourceClock);

		decoupledClock->ChangeSource(adjustableClock);

		return 0;
	});

	timeController = rulesetExecutor->CreateTimeController();
	timeController->ImportWorkingSm(workingSmValue);
	timeController->SetGameClock(offsetClock);
	timeController->SetAudioClock(adjustableClock);

	AddChild(timeController);

	// 把time controller下面所有東西的clock都改成由time controller控制的clock
	Container* container = new Container();
	timeController->AddChild(container);
	container->SetClock(offsetClock);
	container->AddChild(rulesetExecutor);


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
