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
			//SetAllChildsIsMaskedForTrigger(); 這行在speed adjuster的on freeze裡面
		}
		else if (!isWaitingFreeze) {
			LOG(LogLevel::Debug) << "MeteorTimeController::OnButtonDown() : restart and freeze 1 sec.";

			speedAdjuster->SetFreezeTime(defaultFreezeTime);
			isWaitingFreeze = true;
			isAdjustAfterPause = false;
		}
		else
			return -1;

	}
	return 0;
}

int MeteorTimeController::onKnobTurn(InputState * inputState, InputKey knob)
{
	if (knob == InputKey::SectionKnob) {
		// 倒數繼續遊戲的時候不准任何其他動作
		if (isWaitingFreeze)
			return 0;

		int turnValue = 0;
		for (int i = 0; i < inputState->GetPanelState()->GetKnobs()->size(); i++)
			if (inputState->GetPanelState()->GetKnobs()->at(i).first == InputKey::SectionKnob)
				turnValue = inputState->GetPanelState()->GetKnobs()->at(i).second;

		if (!GetIsPaused()) {
			Pause();
		}
		else if(!speedAdjuster->GetIsAdjustingTime())
			isAdjustAfterPause = true;

		speedAdjuster->SetSeekTime(turnValue * defaultAdjustTime);

		// 這邊是避免剛好set seek time以後seek time剛好等於0，會造成意外狀況，所以刻意不讓他最後變成0，就隨便加一個數字上去
		if(speedAdjuster->GetSeekTime() == 0)
			speedAdjuster->SetSeekTime(turnValue);

	}


	return 0;
}

/* 改成不繼承meteo action
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