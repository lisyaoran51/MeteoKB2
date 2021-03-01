#include "InstantReplayRecorder.h"

#include "InstantReplayRecorderReceiver.h"

using namespace Instant::Rulesets::Replays;



InstantReplayRecorder::InstantReplayRecorder() : RegisterType("InstantReplayRecorder")
{
}

string InstantReplayRecorder::GetReplayRecorderVersion()
{
	return string("InstantReplayRecorderV1");
}

ReplayRecorderReceiver * InstantReplayRecorder::CreateReceiver()
{
	return new InstantReplayRecorderReceiver();
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

int InstantReplayRecorder::onKeyDown(pair<InstantAction, int> action)
{
	return 0;
}

int InstantReplayRecorder::onKeyUp(InstantAction action)
{
	return 0;
}

int InstantReplayRecorder::onButtonDown(InstantAction action)
{
	return 0;
}

int InstantReplayRecorder::onButtonUp(InstantAction action)
{
	return 0;
}

int InstantReplayRecorder::onKnobTurn(pair<InstantAction, int> action)
{
	return 0;
}

int InstantReplayRecorder::onSlide(pair<InstantAction, int> action)
{
	return 0;
}
