#include "InstantReplayFrame.h"

using namespace Instant::Rulesets::Replays;


InstantReplayFrame::InstantReplayFrame(double t, InstantAction mAction, int v, bool down): ReplayFrame(t)
{
	action = mAction;
	volume = v;
	isPressingDown = down;
}

string InstantReplayFrame::ToString()
{
	return string();
}

InstantAction InstantReplayFrame::GetInstantAction()
{
	return action;
}

int InstantReplayFrame::GetVolume()
{
	return volume;
}

bool InstantReplayFrame::GetIsPressingDown()
{
	return isPressingDown;
}
