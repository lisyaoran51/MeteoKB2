#include "ReplayFrame.h"


using namespace Games::Rulesets::Replays;


ReplayFrame::ReplayFrame(double t)
{
	time = t;
}

double ReplayFrame::GetStartTime()
{
	return time;
}
