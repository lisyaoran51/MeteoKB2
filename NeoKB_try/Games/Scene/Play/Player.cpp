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
	LOG(LogLevel::Info) << "Player::load : start loading the player and reading the sm and ruleset from working sm.";

	WorkingSm* workingSmValue = workingSm.GetValue();
	// �o�ӬO���g��ruleset �A����n�令�q�ɮ�Ū
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

	LOG(LogLevel::Fine) << "Player::load : create clocks.";
	if (workingSmValue->GetTrack() == nullptr)
		adjustableClock = new StopwatchClock();
	else
		adjustableClock = workingSmValue->GetTrack();

	LOG(LogLevel::Fine) << "Player::load : create decoupled clocks.";
	// �o�ӬO��track����H��A�٥i�H�~��]����
	decoupledClock = new DecoupledInterpolatingFramedClock();
	decoupledClock->SetIsCoupled(false);

	offsetClock = new OffsetFramedClock(decoupledClock);

	// TODO: ��config�̭���offset�Moffset clock��offset bind�b�@�_�A��config�i�H�վ�offset

	LOG(LogLevel::Fine) << "Player::load : scheduler = [" << GetScheduler() << "].";

	GetScheduler()->AddTask([=]() {

		LOG(LogLevel::Info) << "Player::load : scheduled task to change source to track.";
		decoupledClock->Reset();

		// TODO: ��mod�[�JadjustableClock��
		// foreach (var mod in working.Mods.Value.OfType<IApplicableToClock>())
		//     mod.ApplyToClock(adjustableSourceClock);

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
	// ��time controller�U���Ҧ��F�誺clock���令��time controller���clock
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
