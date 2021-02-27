#include "RecordReplayFrame.h"

using namespace Record::Rulesets::Replays;


RecordReplayFrame::RecordReplayFrame(double t, RecordAction mAction, int v, bool down): ReplayFrame(t)
{
	action = mAction;
	volume = v;
	isPressingDown = down;
}

string RecordReplayFrame::ToString()
{
	return string();
}

RecordAction RecordReplayFrame::GetRecordAction()
{
	return action;
}

int RecordReplayFrame::GetVolume()
{
	return volume;
}

bool RecordReplayFrame::GetIsPressingDown()
{
	return isPressingDown;
}
