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

	/* ����configuration��modifier */
	for (int i = 0; i < workingSmValue->GetModifiers()->GetValue()->size(); i++) {
		if (dynamic_cast<ConfigurationModifier*>(workingSmValue->GetModifiers()->GetValue()->at(i))) {
			ConfigurationModifier* cModifier = dynamic_cast<ConfigurationModifier*>(workingSmValue->GetModifiers()->GetValue()->at(i));
			cModifier->ApplyToConfigurationManager(GetCache<ConfigManager>(cModifier->GetConfigManagerName()));
		}
	}

	LOG(LogLevel::Finer) << "Player::load() : after apply modifiers.";
	LOG(LogLevel::Finer) << "Player::load() : modifiers [" << workingSmValue->GetModifiers()->GetValue()->size() << "].";

	// �o�ӬO���g��ruleset �A����n�令�q�ɮ�Ū(���U�@�樺��)
	//rulesetInfo.SetValue(new RulesetInfo("MeteorRuleset", 1));

	try {
		rulesetInfo.SetValue(workingSm.GetValue()->GetSm()->GetSmInfo()->rulesetInfo);
	}
	catch (exception& e) {
		LOG(LogLevel::Info) << "Player::load() : create sm failed. go back to song select. error message:" << e.what();

		// �o�����ӭn��error handling�����

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

	// ��D�����[�Atime controller�W
	//rulesetExecutor->SetLeaveGameFunction(bind(&Player::onQuiting, this));
	//rulesetExecutor->SetRestartGameFunction(bind(&Player::onRestarting, this));
	//rulesetExecutor->SetEndGameFunction(bind(&Player::onCompletion, this));
	
	/***
	Sm<Event>* sm = workingSm->GetSm();
	 
	if (!rulesetInfo)
		rulesetInfo = sm->GetRulesetInfo();
	***/

	// �o�����get track
	//LOG(LogLevel::Fine) << "Player::load : create clocks. track = [" << workingSmValue->GetTrack() << "].";
	//if (workingSmValue->GetTrack() == nullptr) {
	//	adjustableClock = new StopwatchClock();
	//	LOG(LogLevel::Warning) << "Player::load : no music found. track = [" << workingSmValue->GetTrack() << "].";
	//}
	//else {
	//	adjustableClock = workingSmValue->GetTrack();
	//	// �n�Aruleset executor����event processor filter��sm sound event��filter��
	//}
	adjustableClock = new StopwatchClock();


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
			
		// output �C���}�l
		LOG(LogLevel::Debug) << "Player::load : scheduled task end.";

		return 0;
	});


	LOG(LogLevel::Fine) << "Player::load : adding time controller, ruleset executor. clock = [" << GetClock() << "].";
	AddChild(timeController);

	// replay recorder�n�hget time controller�A�ҥHtime controller�n��add child�A�A�Ӥ~�Oreplay recorder
	// ����playfield�̪�piano controller�n��replay recorder�A�ҥH�n�ArulesetExecutor���e��add child replay recorder
	replayRecorder = rulesetExecutor->CreateReplayRecorder();
	replayRecorder->SetDefaultKeyBindings(rulesetInfo.GetValue());
	AddChild(replayRecorder);


	LOG(LogLevel::Debug) << "Player::load() : time controller added.";
	// ��time controller�U���Ҧ��F�誺clock���令��time controller���clock
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


	// �o��n�ﱼ�A���ӥήɶ��ӧP�_�A�Ӥ��O��judgement�ӧP�_
	//scoreProcessor->AddOnAllJudged(this, bind(&Player::onCompletion, this), "Player::onCompletion"); // ��ܦ��Z����
	timeController->AddOnRetry(this, bind(&Player::onQuiting, this), "Player::onQuiting"); // ���s�}�l
	timeController->AddOnQuit(this, bind(&Player::onRestarting, this), "Player::onRestarting"); // �פ�C���A�����⦨�Z
	timeController->AddOnGameOver(this, bind(&Player::onCompletion, this), "Player::onCompletion"); // �C�������A��ܦ��Z����

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
