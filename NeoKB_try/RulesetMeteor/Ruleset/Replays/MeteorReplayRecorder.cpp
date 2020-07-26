#include "MeteorReplayRecorder.h"

#include "MeteorReplayFrame.h"

using namespace Meteor::Rulesets::Replays;



MeteorReplayRecorder::MeteorReplayRecorder() : RegisterType("MeteorReplayRecorder")
{
}

int MeteorReplayRecorder::OnKeyDown(pair<MeteorAction, int> action)
{

	replay->replayFrames.push_back(new MeteorReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action.first, action.second, true));
	return 0;
}

int MeteorReplayRecorder::OnKeyUp(MeteorAction action)
{
	replay->replayFrames.push_back(new MeteorReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action, -1, false));
	return 0;
}
