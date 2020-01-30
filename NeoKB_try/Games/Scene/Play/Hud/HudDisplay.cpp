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

	// 這邊是把key counter綁到ruleset input manager上，這樣每案下一次案件，key counter就會直接接收到
	// 暫時懶得寫，不知道有什麼用途

	return 0;
}

int HudDisplay::BindTimeController(TimeController * tController)
{
	LOG(LogLevel::Info) << "HudDisplay::BindTimeController : bind controllable clock to control.";

	songProgress->SetTimeSource(tController->GetControllableClock());
	songProgress->SetIsAllowSeek(tController->GetIsAllowSeek()); // 允許搜尋時間
	
	// 接下來這個，是song progress下面有一個progress bar，他可以接收調整時間，然後傳到song progress，再用song progress控制controllable clock
	// 這個等到有手機連線的時候再寫就好
	songProgress->AddOnSeek(songProgress->GetTimeSource(), bind(&AdjustableClock::Seek, songProgress->GetTimeSource(), placeholders::_1), "DecoupledInterpolatingFramedClock::Seek");

	LOG(LogLevel::Debug) << "HudDisplay::BindTimeController : bind over.";
	return 0;
}
