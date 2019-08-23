#ifndef THROTTLED_FRAMED_CLOCK_H
#define THROTTLED_FRAMED_CLOCK_H

#include "FramedClock.h"
#include "FrameTimeInfo.h"

namespace Framework {
namespace Timing {

	/// <summary>
	/// Takes a clock source and separates time reading on a per-frame level.
	/// The CurrentTime value will only change on initial construction and whenever ProcessFrame is run.
	/// </summary>
	class ThrottledFramedClock : virtual public FramedClock {

		int throttle();

	public:

		ThrottledFramedClock(Clock* s = nullptr);

	protected:




	};




}}



#endif