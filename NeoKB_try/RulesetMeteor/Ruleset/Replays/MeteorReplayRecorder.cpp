#include "MeteorReplayRecorder.h"

#include "MeteorReplayFrame.h"

using namespace Meteor::Rulesets::Replays;



MeteorReplayRecorder::MeteorReplayRecorder() : RegisterType("MeteorReplayRecorder")
{
}

string MeteorReplayRecorder::GetReplayRecorderVersion()
{
	return string("MeteorReplayRecorderV1");
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

int MeteorReplayRecorder::OnButtonDown(MeteorAction action)
{
	return 0;
}

int MeteorReplayRecorder::OnButtonUp(MeteorAction action)
{
	return 0;
}

int MeteorReplayRecorder::OnKnobTurn(pair<MeteorAction, int> action)
{
	return 0;
}

int MeteorReplayRecorder::OnSlide(pair<MeteorAction, int> action)
{
	return 0;
}
