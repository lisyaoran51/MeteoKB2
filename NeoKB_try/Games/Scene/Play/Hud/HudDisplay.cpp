#include "HudDisplay.h"

#include "JudgementFlasher.h"
#include "../../../../Util/DataStructure/ActionList.h"



using namespace Games::Scenes::Play::Hud;
using namespace Util::DataStructure;




HudDisplay::HudDisplay(): RegisterType("HudDisplay")
{

	scoreCounter = new RollingCounter<double>(0);
	comboCounter = new RollingCounter<int>(0);
	accuracyCounter = new RollingCounter<double>(1);


}

int HudDisplay::BindScoreProcessor(ScoreProcessor * sProcessor)
{
	scoreCounter->GetCurrentValue()->BindTo(sProcessor->GetTotalScore());
	accuracyCounter->GetCurrentValue()->BindTo(sProcessor->GetAccuracy());
	comboCounter->GetCurrentValue()->BindTo(sProcessor->GetCombo());

	sProcessor->AddOnNewJudgement(judgementFlasher, bind(&JudgementFlasher::FlashJudgement, judgementFlasher, placeholders::_1), "JudgementFlasher::FlashJudgement");
	LOG(LogLevel::Depricated) << "HudDisplay::BindScoreProcessor : after bind judgement flasher ...";
	return 0;
}

int HudDisplay::BindRulesetExecutor(RulesetExecutor<Event>* rExecutor)
{

	// �o��O��key counter�j��ruleset input manager�W�A�o�˨C�פU�@���ץ�Akey counter�N�|����������
	// �Ȯ��i�o�g�A�����D������γ~

	return 0;
}

int HudDisplay::BindTimeController(TimeController * tController)
{
	LOG(LogLevel::Info) << "HudDisplay::BindTimeController : bind controllable clock to control.";

	songProgress->SetTimeSource(tController->GetControllableClock());
	songProgress->SetIsAllowSeek(tController->GetIsAllowSeek()); // ���\�j�M�ɶ�
	
	// ���U�ӳo�ӡA�Osong progress�U�����@��progress bar�A�L�i�H�����վ�ɶ��A�M��Ǩ�song progress�A�A��song progress����controllable clock
	// �o�ӵ��즳����s�u���ɭԦA�g�N�n
	songProgress->AddOnSeek(songProgress->GetTimeSource(), bind(&AdjustableClock::Seek, songProgress->GetTimeSource(), placeholders::_1), "DecoupledInterpolatingFramedClock::Seek");

	LOG(LogLevel::Debug) << "HudDisplay::BindTimeController : bind over.";
	return 0;
}
