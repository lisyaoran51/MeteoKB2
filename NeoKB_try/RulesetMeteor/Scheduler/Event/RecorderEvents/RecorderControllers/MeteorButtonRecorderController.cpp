#include "MeteorButtonRecorderController.h"

#include "../../../../Ruleset/Replays/MeteorReplayFrame.h"


using namespace Meteor::Schedulers::Events::RecorderEvents::RecorderControllers;
using namespace Meteor::Rulesets::Replays;


int MeteorButtonRecorderController::load()
{
	meteorReplayRecorder = dynamic_cast<MeteorReplayRecorder*>(replayRecorder);
	return 0;
}

MeteorButtonRecorderController::MeteorButtonRecorderController() : RegisterType("MeteorButtonRecorderController")
{
	registerLoad(bind((int(MeteorButtonRecorderController::*)())&MeteorButtonRecorderController::load, this));
}

int MeteorButtonRecorderController::implementAddReplay(EventProcessor<Event>* e)
{
	LOG(LogLevel::Depricated) << "MeteorButtonRecorderController::implementAddReplay : make pedal [" << dynamic_cast<MeteorButtonRecorderEvent*>(e->GetEvent())->GetInput().second << "] on recorder [" << meteorReplayRecorder << "].";

	if (!meteorReplayRecorder)
		return -1;

	if (!dynamic_cast<MeteorButtonRecorderEvent*>(e->GetEvent()))
		return -1;

	MeteorButtonRecorderEvent* buttonRecorderEvent = dynamic_cast<MeteorButtonRecorderEvent*>(e->GetEvent());

	if (buttonRecorderEvent->GetInput().first == InputKey::SustainPedal) {
		if (buttonRecorderEvent->GetInput().second >= 0) {
			meteorReplayRecorder->OnDirectRecordButtonDown(buttonRecorderEvent->GetInput().first);
		}
		else {
			meteorReplayRecorder->OnDirectRecordButtonUp(buttonRecorderEvent->GetInput().first);
		}
	}
	else if (buttonRecorderEvent->GetInput().first == InputKey::LowerOctave) {

		meteorReplayRecorder->OnDirectRecordButtonDown(buttonRecorderEvent->GetInput().first);

	}
	else if (buttonRecorderEvent->GetInput().first == InputKey::RaiseOctave) {

		meteorReplayRecorder->OnDirectRecordButtonDown(buttonRecorderEvent->GetInput().first);

	}


	return 0;
}
