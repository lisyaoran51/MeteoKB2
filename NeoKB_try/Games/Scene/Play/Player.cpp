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
	// �o�ӬO���g��ruleset �A����n�令�q�ɮ�Ū(���U�@�樺��)
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
		// �n�Aruleset executor����event processor filter��sm sound event��filter��
	}


	LOG(LogLevel::Fine) << "Player::load : create decoupled clocks.";
	// �o�ӬO��track����H��A�٥i�H�~��]����
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

	// TODO: ��config�̭���offset�Moffset clock��offset bind�b�@�_�A��config�i�H�վ�offset

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

		/* �˼� */
		if (workingSmValue->GetSm()->GetSmInfo()->countdown)
			timeController->JumpTo(-4.0 / timeController->GetRate());
		else
			timeController->JumpTo(-1.0 / timeController->GetRate());
			
		
		LOG(LogLevel::Debug) << "Player::load : scheduled task end.";

		return 0;
	});


	LOG(LogLevel::Fine) << "Player::load : adding time controller, ruleset executor. clock = [" << GetClock() << "].";
	AddChild(timeController);
	// ��time controller�U���Ҧ��F�誺clock���令��time controller���clock
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

	scoreProcessor->AddOnAllJudged(this, bind(&Player::onCompletion, this), "Player::onCompletion"); // ��ܦ��Z����

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

		// TODO: �o�����ӭn��replay���@�Uclone�A�����|����}�G�C�p�Gclone����score�b�Ѻc���ɭԴN�n�R��replay
		score->replay = replayRecorder->GetReplay();

		//score->user = local user �A�ݤ@�U���g

		score->smInfo = workingSm.GetValue()->GetSm()->GetSmInfo();
		score->rulesetInfo = rulesetInfo.GetValue();

		// osu �A�o�䤣�|�h�[�Jreplay�A�o���sreplay�M�i�Jresult�O���}������
		Push(rulesetExecutor->CreateResult(score));
		return 0;
	}, 3);


	return 0;
}

int Player::onExiting(Scene * lastScene)
{
	return 0;
}
