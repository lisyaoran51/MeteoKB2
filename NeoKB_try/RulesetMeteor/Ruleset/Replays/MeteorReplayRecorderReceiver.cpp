#include "MeteorReplayRecorderReceiver.h"


using namespace Meteor::Rulesets::Replays;


MeteorReplayRecorderReceiver::MeteorReplayRecorderReceiver() : RegisterType("MeteorReplayRecorderReceiver")
{
	LOG(LogLevel::Debug) << "MeteorReplayRecorderReceiver::MeteorReplayRecorderReceiver() : constructing.";
}
