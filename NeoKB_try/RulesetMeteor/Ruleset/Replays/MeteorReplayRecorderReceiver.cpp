#include "MeteorReplayRecorderReceiver.h"


using namespace Meteor::Rulesets::Replays;


int MeteorReplayRecorderReceiver::load()
{
	meteorReplayRecorder = dynamic_cast<MeteorReplayRecorder*>(replayRecorder);

	return 0;
}

MeteorReplayRecorderReceiver::MeteorReplayRecorderReceiver() : RegisterType("MeteorReplayRecorderReceiver")
{
	LOG(LogLevel::Depricated) << "MeteorReplayRecorderReceiver::MeteorReplayRecorderReceiver() : constructing.";
	registerLoad(bind((int(MeteorReplayRecorderReceiver::*)())&MeteorReplayRecorderReceiver::load, this));

}

int MeteorReplayRecorderReceiver::update()
{
	meteorReplayRecorder->PassCurrentTime(GetClock()->GetCurrentTime());

	return 0;
}
