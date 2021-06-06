#include "Player.h"

#include <string>
#include "../../Sheetmusic/Sheetmusic.h"
#include "../../Ruleset/Modifiers/InstrumentModifier.h"

#include "../../Ruleset/Modifiers/TimeControllerModifier.h"
#include "../Results/Result.h"
#include "../../Ruleset/Modifiers/ConfigurationModifier.h"
#include "../../MeteoGame.h"



using namespace std;
using namespace Games::Scenes::Play;
using namespace Games::Rulesets;
using namespace Games::Sheetmusics;
using namespace Games;
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

	MeteoGame * g = GetCache<MeteoGame>("MeteoGame");
	if (!g)
		throw runtime_error("Player::load() : Game not found in cache.");

	return load(m, i, g);
}

int Player::load(MeteoConfigManager* m, Instrument* instru, MeteoGame * g)
{
	meteoGame = g;
	LOG(LogLevel::Info) << "Player::load() : start loading the player and reading the sm and ruleset from working sm.";

	WorkingSm* workingSmValue = workingSm.GetValue();

	if (workingSm.GetValue() == nullptr) {
		LOG(LogLevel::Error) << "Player::load() : no working sm.";
		throw runtime_error("Player::load() : no working sm.");
	}

	/* 抓更改configuration的modifier */
	for (int i = 0; i < workingSmValue->GetModifiers()->GetValue()->size(); i++) {
		if (dynamic_cast<ConfigurationModifier*>(workingSmValue->GetModifiers()->GetValue()->at(i))) {
			ConfigurationModifier* cModifier = dynamic_cast<ConfigurationModifier*>(workingSmValue->GetModifiers()->GetValue()->at(i));
			cModifier->ApplyToConfigurationManager(GetCache<ConfigManager>(cModifier->GetConfigManagerName()));
		}
	}

	LOG(LogLevel::Finer) << "Player::load() : after apply modifiers.";
	LOG(LogLevel::Finer) << "Player::load() : modifiers [" << workingSmValue->GetModifiers()->GetValue()->size() << "].";

	// 這個是先寫死ruleset ，之後要改成從檔案讀(像下一行那樣)
	//rulesetInfo.SetValue(new RulesetInfo("MeteorRuleset", 1));

	try {
		rulesetInfo.SetValue(workingSm.GetValue()->GetSm()->GetSmInfo()->rulesetInfo);
	}
	catch (exception& e) {
		LOG(LogLevel::Info) << "Player::load() : create sm failed. go back to song select. error message:" << e.what();

		// 這邊應該要丟error handling給手機

		Exit();
		return -1;
	}

	// debug
	for (int i = 0; i < workingSm.GetValue()->GetSm()->GetEvents()->size(); i++) {
		LOG(LogLevel::Depricated) << "Player::load : sm event [" << workingSm.GetValue()->GetSm()->GetEvents()->at(i)->GetTypeName() << "] at [" << workingSm.GetValue()->GetSm()->GetEvents()->at(i)->GetStartTime() << "].";
	}

	ruleset = rulesetInfo.GetValue()->CreateRuleset();
	LOG(LogLevel::Fine) << "Player::load : create ruleset executor.";
	rulesetExecutor = ruleset->CreateRulesetExecutor(workingSm.GetValue());

	// 改道直接加再time controller上
	//rulesetExecutor->SetLeaveGameFunction(bind(&Player::onQuiting, this));
	//rulesetExecutor->SetRestartGameFunction(bind(&Player::onRestarting, this));
	//rulesetExecutor->SetEndGameFunction(bind(&Player::onCompletion, this));
	
	/***
	Sm<Event>* sm = workingSm->GetSm();
	 
	if (!rulesetInfo)
		rulesetInfo = sm->GetRulesetInfo();
	***/

	// 這邊取消get track
	//LOG(LogLevel::Fine) << "Player::load : create clocks. track = [" << workingSmValue->GetTrack() << "].";
	//if (workingSmValue->GetTrack() == nullptr) {
	//	adjustableClock = new StopwatchClock();
	//	LOG(LogLevel::Warning) << "Player::load : no music found. track = [" << workingSmValue->GetTrack() << "].";
	//}
	//else {
	//	adjustableClock = workingSmValue->GetTrack();
	//	// 要再ruleset executor李把event processor filter的sm sound event給filter調
	//}
	adjustableClock = new StopwatchClock();


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
			
		// output 遊戲開始
		LOG(LogLevel::Debug) << "Player::load : scheduled task end.";

		return 0;
	});


	LOG(LogLevel::Fine) << "Player::load : adding time controller, ruleset executor. clock = [" << GetClock() << "].";
	AddChild(timeController);

	// replay recorder要去get time controller，所以time controller要先add child，再來才是replay recorder
	// 之後playfield裡的piano controller要抓replay recorder，所以要再rulesetExecutor之前先add child replay recorder
	replayRecorder = rulesetExecutor->CreateReplayRecorder();
	replayRecorder->SetDefaultKeyBindings(rulesetInfo.GetValue());
	AddChild(replayRecorder);


	LOG(LogLevel::Debug) << "Player::load() : time controller added.";
	// 把time controller下面所有東西的clock都改成由time controller控制的clock
	Container* container = new Container();
	timeController->AddChild(container);
	LOG(LogLevel::Debug) << "Player::load() : set offset clock.";
	container->SetClock(offsetClock);
	LOG(LogLevel::Debug) << "Player::load() : add ruleset executor.";
	container->AddChild(rulesetExecutor);

	LOG(LogLevel::Debug) << "Player::load() : ruleset executor loaded.";


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


	// 這行要改掉，應該用時間來判斷，而不是用judgement來判斷
	//scoreProcessor->AddOnAllJudged(this, bind(&Player::onCompletion, this), "Player::onCompletion"); // 顯示成績結算
	timeController->AddOnRetry(this, bind(&Player::onQuiting, this), "Player::onQuiting"); // 重新開始
	timeController->AddOnQuit(this, bind(&Player::onRestarting, this), "Player::onRestarting"); // 終止遊戲，不結算成績
	timeController->AddOnGameOver(this, bind(&Player::onCompletion, this), "Player::onCompletion"); // 遊戲結束，顯示成績結算

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

	LOG(LogLevel::Depricated) << "Player::~Player : delete its members over.";

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

	MeteoContextBluetoothMessage* message = new MeteoContextBluetoothMessage(MeteoCommand::HardwareCompleteGame);
	message->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
	outputManager->PushMessage(message);


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

int Player::onQuiting()
{
	LOG(LogLevel::Info) << "Player::onQuiting : quit game. go back to song select";
	Exit();
	return 0;
}

int Player::onRestarting()
{
	LOG(LogLevel::Info) << "Player::onRestarting : go back to player loader.";
	Restart();
	return 0;
}

int Player::onExiting(Scene * lastScene)
{
	meteoGame->SetConnectState();

	MeteoScene::onExiting(lastScene);
	return 0;
}
