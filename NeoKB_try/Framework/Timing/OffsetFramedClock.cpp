#include "OffsetFramedClock.h"

#include "../../Util/Log.h"


using namespace Framework::Timing;
using namespace Util;



OffsetFramedClock::OffsetFramedClock(Clock * s) : FramedClock(s)
{
	LOG(LogLevel::Depricated) << "OffsetFramedClock::OffsetFramedClock() : address = [" << this << "].";
	offset = 0;
}

double OffsetFramedClock::GetCurrentTime()
{
	return FramedClock::GetCurrentTime() + offset;
}

int OffsetFramedClock::SetOffset(double o)
{
	lastFrameTime += o - offset;
	offset = o;
	return 0;
}

double OffsetFramedClock::GetOffset()
{
	return offset;
}
