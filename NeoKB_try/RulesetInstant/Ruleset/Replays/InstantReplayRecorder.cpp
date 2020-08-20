#include "InstantReplayRecorder.h"

using namespace Instant::Rulesets::Replays;



InstantReplayRecorder::InstantReplayRecorder() : RegisterType("InstantReplayRecorder")
{
}

string InstantReplayRecorder::GetReplayRecorderVersion()
{
	return string("InstantReplayRecorderV1");
}

int InstantReplayRecorder::OnKeyDown(pair<InstantAction, int> action)
{
	return 0;
}

int InstantReplayRecorder::OnKeyUp(InstantAction action)
{
	return 0;
}

int InstantReplayRecorder::OnButtonDown(InstantAction action)
{
	return 0;
}

int InstantReplayRecorder::OnButtonUp(InstantAction action)
{
	return 0;
}

int InstantReplayRecorder::OnKnobTurn(pair<InstantAction, int> action)
{
	return 0;
}

int InstantReplayRecorder::OnSlide(pair<InstantAction, int> action)
{
	return 0;
}