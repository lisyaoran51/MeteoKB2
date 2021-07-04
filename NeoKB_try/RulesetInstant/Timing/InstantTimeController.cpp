#include "InstantTimeController.h"

#include "../Scheduler/Event/Effect/InstantFallEffectMapper.h"



using namespace Instant::Timing;
using namespace std;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Instant::Schedulers::Events::Effects;



int InstantTimeController::load()
{
	LOG(LogLevel::Info) << "InstantTimeController::load() : loaded. ";
	
	return 0;
}

InstantTimeController::InstantTimeController() : RegisterType("InstantTimeController")
{
	registerLoad(bind(static_cast<int(InstantTimeController::*)(void)>(&InstantTimeController::load), this));
}

int InstantTimeController::onMessage(MeteoBluetoothMessage * message)
{
	if (message->GetCommand() == MeteoCommand::AppQuitGame) {

		/* 離開遊戲時將燈光關閉 */
		//SpeedRingPanelMessage* speedRingPanelMessage = new SpeedRingPanelMessage(5);
		//outputManager->PushMessage(speedRingPanelMessage);

	}

	if (message->GetCommand() == MeteoCommand::AppCompleteGame || message->GetCommand() == MeteoCommand::AppRestartGame) {
		return -1;
	}


	return MeteoTimeController::onMessage(message);
}
