#include "MeteorTimeController.h"


using namespace Meteor::Timing;



MeteorTimeController::MeteorTimeController() : RegisterType("MeteorTimeController")
{
}

int MeteorTimeController::onButtonDown(InputState * inputState, InputKey button)
{
	if (button == InputKey::Pause) {
		LOG(LogLevel::Debug) << "MeteorTimeController::OnButtonDown() : get pause button input and pause.";
		if (speedAdjuster->GetIsAdjustingTime())
			return -1;

		if (!GetIsPaused()) {
			Pause();
			LOG(LogLevel::Debug) << "MeteorTimeController::OnButtonDown() : after pause.";
			SetAllChildsIsMaskedForTrigger();
			LOG(LogLevel::Debug) << "MeteorTimeController::OnButtonDown() : after mask.";
		}
		else if (!isWaitingFreeze) {
			speedAdjuster->SetFreezeTime(defaultFreezeTime);
			isWaitingFreeze = true;
		}
	}
	return 0;
}

/* §ï¦¨¤£Ä~©Ómeteo action
map<MeteorAction, InputKey>* MeteorTimeController::GetDefaultkeyBindings()
{
	map<MeteorAction, InputKey>* tempMap = new map<MeteorAction, InputKey>();
	(*tempMap)[MeteorAction::Pause] = InputKey::Pause;
	(*tempMap)[MeteorAction::SectionKnob] = InputKey::SectionKnob;
	(*tempMap)[MeteorAction::SpeedKnob] = InputKey::SpeedKnob;
	(*tempMap)[MeteorAction::Bluetooth] = InputKey::Bluetooth;


	return tempMap;
}
*/

/* meteo action
int MeteorTimeController::reloadMappings()
{
	LOG(LogLevel::Info) << "MeteorTimeController::reloadMappings() : mapping key bindings.";
	map<MeteorAction, InputKey>* tempMap = GetDefaultkeyBindings();
	keyBindings = *tempMap;
	return 0;
}
*/