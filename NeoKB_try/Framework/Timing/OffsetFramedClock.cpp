#include "OffsetFramedClock.h"


using namespace Framework::Timing;



OffsetFramedClock::OffsetFramedClock(Clock * s) : FramedClock(s)
{
	offset = 0;
}

double OffsetFramedClock::GetCurrentTime()
{
	return FramedClock::GetCurrentTime() + offset;
}

int OffsetFramedClock::SetOffset(double o)
{
	offset = o;
	return 0;
}

double OffsetFramedClock::GetOffset()
{
	return 0.0;
}
