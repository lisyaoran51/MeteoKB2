#include "MeteorTimeController.h"


using namespace Meteor::Timing;



MeteorTimeController::MeteorTimeController() : RegisterType("MeteorTimeController")
{
}

map<PianoAction, InputKey>* MeteorTimeController::GetDefaultkeyBindings()
{
	map<PianoAction, InputKey>* tempMap = new map<PianoAction, InputKey>();
	(*tempMap)[PianoAction::Pause] = InputKey::Pause;
	(*tempMap)[PianoAction::SectionKnob] = InputKey::SectionKnob;
	(*tempMap)[PianoAction::SpeedKnob] = InputKey::SpeedKnob;
	(*tempMap)[PianoAction::Bluetooth] = InputKey::Bluetooth;


	return tempMap;
}

int MeteorTimeController::reloadMappings()
{
	map<PianoAction, InputKey>* tempMap = GetDefaultkeyBindings();
	keyBindings = *tempMap;
	delete tempMap;
	return 0;
}
