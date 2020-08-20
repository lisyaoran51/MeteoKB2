#include "MeteorReplayFrame.h"

using namespace Meteor::Rulesets::Replays;


MeteorReplayFrame::MeteorReplayFrame(double t, MeteorAction mAction, int v, bool down): ReplayFrame(t)
{
	action = mAction;
	volume = v;
	isPressingDown = down;
}

string MeteorReplayFrame::ToString()
{
	return string();
}

MeteorAction MeteorReplayFrame::GetMeteorAction()
{
	return action;
}

int MeteorReplayFrame::GetVolume()
{
	return volume;
}

bool MeteorReplayFrame::GetIsPressingDown()
{
	return isPressingDown;
}
