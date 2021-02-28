#include "MeteorButtonRecorderEventProcessor.h"


using namespace Meteor::Schedulers::Events::RecorderEvents;


pair<InputKey, int> MeteorButtonRecorderEventProcessor::GetInput()
{
	return dynamic_cast<MeteorButtonRecorderEvent*>(GetEvent())->GetInput();
}
