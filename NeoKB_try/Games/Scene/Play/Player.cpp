#include "Player.h"

#include <string>
#include "../../Sheetmusic/Sheetmusic.h"
#include "../../Ruleset/Modifiers/InstrumentModifier.h"



using namespace std;
using namespace Games::Scenes::Play;
using namespace Games::Rulesets;
using namespace Games::Sheetmusics;
using namespace Framework::Configurations;
using namespace Rulesets::Modifiers;
using namespace std;


int Player::load()
{
	FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("Player::load() : FrameworkConfigManager not found in cache.");

	Instrument * i = GetCache<Instrument>("Instrument");
	if (!f)
		throw runtime_error("Player::load() : Instrument not found in cache.");

	return load(f, i);
}

int Player::load(FrameworkConfigManager* f, Instrument* instru)
{
	LOG(LogLevel::Info) << "Player::load : start loading the player and reading the sm and ruleset from working sm.";

	WorkingSm* workingSmValue = workingSm.GetValue();
	// 這個是先寫死ruleset ，之後要改成從檔案讀
	//rulesetInfo.SetValue(new RulesetInfo("MeteorRuleset", 1));
	rulesetInfo.SetValue(workingSm.GetValue()->GetSm()->GetSmInfo()->rulesetInfo);
	ruleset = rulesetInfo.GetValue()->CreateRuleset();
	LOG(LogLevel::Fine) << "Player::load : create ruleset executor.";
	rulesetExecutor = ruleset->CreateRulesetExecutor(workingSm.GetValue());

	/***
	Sm<Event>* sm = workingSm->GetSm();
	 
	if (!rulesetInfo)
		rulesetInfo = sm->GetRulesetInfo();
	***/

	LOG(LogLevel::Fine) << "Player::load : create clocks. track = [" << workingSmValue->GetTrack() << "].";
	if (workingSmValue->GetTrack() == nullptr) {
		adjustableClock = new StopwatchClock();
		LOG(LogLevel::Warning) << "Player::load : no music found. track = [" << workingSmValue->GetTrack() << "].";
	}
	else
		adjustableClock = workingSmValue->GetTrack();


	LOG(LogLevel::Fine) << "Player::load : create decoupled clocks.";
	// 這個是當track停止以後，還可以繼續跑的鍾
	decoupledClock = new DecoupledInterpolatingFramedClock();
	decoupledClock->SetIsCoupled(false);

	offsetClock = new OffsetFramedClock(decoupledClock);

	// TODO: 把config裡面的offset和offset clock的offset bind在一起，讓config可以調整offset

	LOG(LogLevel::Fine) << "Player::load : scheduler = [" << GetScheduler() << "].";

	GetScheduler()->AddTask([=]() {

		LOG(LogLevel::Info) << "Player::load : scheduled task to change source to track [" << adjustableClock << "].";
		adjustableClock->Reset();

		// TODO: 把mod加入adjustableClock裡
		// foreach (var mod in working.Mods.Value.OfType<IApplicableToClock>())
		//     mod.ApplyToClock(adjustableSourceClock);
		LOG(LogLevel::Debug) << "Player::load : reseted.";
		decoupledClock->ChangeSource(adjustableClock);
		
		LOG(LogLevel::Debug) << "Player::load : scheduled task end.";

		return 0;
	});

	LOG(LogLevel::Fine) << "Player::load : create time controller.";
	timeController = rulesetExecutor->CreateTimeController();
	LOG(LogLevel::Fine) << "Player::load : set controllable clock.";
	//timeController->ImportWorkingSm(workingSmValue);
	timeController->SetControllableClock(decoupledClock);
	LOG(LogLevel::Fine) << "Player::load : set speed adjuster.";
	timeController->SetSpeedAdjuster(rulesetExecutor->CreateSpeedAdjuster());


	LOG(LogLevel::Fine) << "Player::load : adding time controller, ruleset executor. clock = [" << GetClock() << "].";
	AddChild(timeController);
	// 把time controller下面所有東西的clock都改成由time controller控制的clock
	Container* container = new Container();
	timeController->AddChild(container);
	container->SetClock(offsetClock);
	container->AddChild(rulesetExecutor);


	instrument = instru;
	modifiers = workingSmValue->GetModifiers()->GetValue();
	LOG(LogLevel::Debug) << "Player::load : get [" << modifiers->size() << "] instrument modifier into [" << instrument << "].";
	for (int i = 0; i < modifiers->size(); i++) {

		if (dynamic_cast<InstrumentModifier*>(modifiers->at(i))) {
			dynamic_cast<InstrumentModifier*>(modifiers->at(i))->ApplyToInstrument(instrument);
		}

	}
	

	hudDisplay = new HudDisplay();

	scoreProcessor = rulesetExecutor->CreateScoreProcessor();

	hudDisplay->BindScoreProcessor(scoreProcessor);
	hudDisplay->BindRulesetExecutor(rulesetExecutor);
	hudDisplay->BindTimeController(timeController);

	scoreProcessor->AddOnAllJudged(this, bind(&Player::onCompletion, this), "Player::onCompletion"); // 顯示成績結算

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

int Player::onCompletion()
{
	return 0;
}
