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

	// �o�ӬO��track����H��A�٥i�H�~��]����
	decoupledClock = new DecoupledInterpolatingFramedClock();
	decoupledClock->SetIsCoupled(false);

	offsetClock = new OffsetFramedClock(decoupledClock);

	// TODO: ��config�̭���offset�Moffset clock��offset bind�b�@�_�A��config�i�H�վ�offset

	GetScheduler()->AddTask([=]() {
		decoupledClock->Reset();

		// TODO: ��mod�[�JadjustableClock��
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
