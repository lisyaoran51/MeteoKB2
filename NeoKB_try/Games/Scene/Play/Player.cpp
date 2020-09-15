#include "Player.h"

#include <string>
#include "../../Sheetmusic/Sheetmusic.h"
#include "../../Ruleset/Modifiers/InstrumentModifier.h"

#include "../../Ruleset/Modifiers/TimeControllerModifier.h"
#include "../Results/Result.h"



using namespace std;
using namespace Games::Scenes::Play;
using namespace Games::Rulesets;
using namespace Games::Sheetmusics;
using namespace Framework::Configurations;
using namespace Rulesets::Modifiers;
using namespace std;
using namespace Games::Scenes::Results;


int Player::load()
{
	MeteoConfigManager * m = GetCache<MeteoConfigManager>("MeteoConfigManager");
	if (!m)
		throw runtime_error("Player::load() : MeteoConfigManager not found in cache.");

	Instrument * i = GetCache<Instrument>("Instrument");
	if (!i)
		throw runtime_error("Player::load() : Instrument not found in cache.");

	return load(m, i);
}

int Player::load(MeteoConfigManager* m, Instrument* instru)
{
	LOG(LogLevel::Info) << "Player::load : start loading the player and reading the sm and ruleset from working sm.";

	WorkingSm* workingSmValue = workingSm.GetValue();
	// 這個是先寫死ruleset ，之後要改成從檔案讀(像下一行那樣)
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
	else {
		adjustableClock = workingSmValue->GetTrack();
		// 要再ruleset executor李把event processor filter的sm sound event給filter調
	}


	LOG(LogLevel::Fine) << "Player::load : create decoupled clocks.";
	// 這個是當track停止以後，還可以繼續跑的鍾
	decoupledClock = new DecoupledInterpolatingFramedClock();
	decoupledClock->SetIsCoupled(false);

	offsetClock = new OffsetFramedClock(decoupledClock);

	if (m->Get(MeteoSetting::AudioOffset, &audioOffset)) {
		offsetClock->SetOffset(audioOffset);
	}



	LOG(LogLevel::Fine) << "Player::load : create time controller.";
	timeController = rulesetExecutor->CreateTimeController();
	LOG(LogLevel::Fine) << "Player::load : set controllable clock.";
	//timeController->ImportWorkingSm(workingSmValue);
	timeController->SetControllableClock(decoupledClock);
	LOG(LogLevel::Fine) << "Player::load : set speed adjuster.";
	timeController->SetSpeedAdjuster(speedAdjuster = rulesetExecutor->CreateSpeedAdjuster());

	// TODO: 把config裡面的offset和offset clock的offset bind在一起，讓config可以調整offset

	LOG(LogLevel::Fine) << "Player::load : scheduler = [" << GetScheduler() << "].";

	GetScheduler()->AddTask([=]() {

		LOG(LogLevel::Info) << "Player::load : scheduled task to change source to track [" << adjustableClock << "].";
		adjustableClock->Reset();

		LOG(LogLevel::Debug) << "Player::load : reseted.";
		decoupledClock->ChangeSource(adjustableClock);

		for (int i = 0; i < workingSmValue->GetModifiers()->GetValue()->size(); i++) {
			if (dynamic_cast<TimeControllerModifier*>(workingSmValue->GetModifiers()->GetValue()->at(i))) {
				dynamic_cast<TimeControllerModifier*>(workingSmValue->GetModifiers()->GetValue()->at(i))->ApplyToTimeController(timeController);
			}

		}

		/* 倒數 */
		if (workingSmValue->GetSm()->GetSmInfo()->countdown)
			timeController->JumpTo(-4.0 / timeController->GetRate());
		else
			timeController->JumpTo(-1.0 / timeController->GetRate());
			
		
		LOG(LogLevel::Debug) << "Player::load : scheduled task end.";

		return 0;
	});


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

	replayRecorder = rulesetExecutor->CreateReplayRecorder();
	AddChild(replayRecorder);

	scoreProcessor->AddOnAllJudged(this, bind(&Player::onCompletion, this), "Player::onCompletion"); // 顯示成績結算

	return 0;
}

Player::Player(): RegisterType("Player"), MeteoScene()
{
	registerLoad(bind((int(Player::*)())&Player::load, this));
}

Player::~Player()
{
	/*
	vector<ChildAddable*>* deletedChilds = DeleteChilds();
	delete deletedChilds;
	*/
	LOG(LogLevel::Debug) << "Player::~Player : delete its members.";

	if (ruleset) {
		delete ruleset;
		ruleset = nullptr;
	}

	if (speedAdjuster) {
		delete speedAdjuster;
		speedAdjuster = nullptr;
	}
	if (offsetClock) {
		delete offsetClock;
		offsetClock = nullptr;
	}
	if (decoupledClock) {
		delete decoupledClock;
		decoupledClock = nullptr;
	}
	if (adjustableClock) {
		delete adjustableClock;
		adjustableClock = nullptr;
	}


	if (hudDisplay) {
		delete hudDisplay;
		hudDisplay = nullptr;
	}
	if (scoreProcessor) {
		delete scoreProcessor;
		scoreProcessor = nullptr;
	}

	LOG(LogLevel::Debug) << "Player::~Player : delete its members over.";

}

int Player::Restart()
{
	SetIsValidForResume(false);
	onRestartRequested.Trigger();
	Exit();
	return 0;
}

int Player::onCompletion()
{

	LOG(LogLevel::Info) << "Player::onCompletion : Game end. Go to result.";
	SetIsValidForResume(false);


	GetScheduler()->AddDelayedTask([=] {
		
		LOG(LogLevel::Info) << "Player::onCompletion : push to result.";

		Score* score = new Score();

		scoreProcessor->PopulateScore(score);

		// TODO: 這邊應該要把replay做一下clone，城市會比較漂亮。如果clone的話score在解構的時候就要刪掉replay
		score->replay = replayRecorder->GetReplay();

		//score->user = local user 再看一下怎麼寫

		score->smInfo = workingSm.GetValue()->GetSm()->GetSmInfo();
		score->rulesetInfo = rulesetInfo.GetValue();

		// osu 再這邊不會去加入replay，她錄製replay和進入result是分開的兩件事
		Push(rulesetExecutor->CreateResult(score));
		return 0;
	}, 3);


	return 0;
}

int Player::onExiting(Scene * lastScene)
{
	return 0;
}
