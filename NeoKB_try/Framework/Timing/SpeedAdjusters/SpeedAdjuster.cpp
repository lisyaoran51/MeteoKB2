#include "SpeedAdjuster.h"


using namespace Framework::Timing::SpeedAdjusters;


SpeedAdjuster::SpeedAdjuster(): RegisterType("SpeedAdjuster")
{
}

int SpeedAdjuster::Pause()
{
	onAdjustFreeze.Trigger();
	return 0;
}

int SpeedAdjuster::Resume()
{
	onAdjustFreezeEnd.Trigger();
	return 0;
}

