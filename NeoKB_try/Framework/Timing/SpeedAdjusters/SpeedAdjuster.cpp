#include "SpeedAdjuster.h"


using namespace Framework::Timing::SpeedAdjusters;


SpeedAdjuster::SpeedAdjuster()
{
	seekTime = 0;
	accumulatedElapsedTime = 0;
}

int SpeedAdjuster::Reset()
{
	accumulatedElapsedTime = 0;
	return 0;
}

int SpeedAdjuster::SetSeekTime(double sTime)
{
	seekTime = sTime;
	return 0;
}

double SpeedAdjuster::GetSeekTime()
{
	return seekTime;
}
