#ifndef STOPWATCH_CLOCK_H
#define STOPWATCH_CLOCK_H

#include "FrameBasedClock.h"
#include "FrameTimeInfo.h"
#include "FramedClock.h"

namespace Framework {
namespace Timing {

	/// <summary>
	/// 裡面有一個碼表當做source，可以開關
	/// </summary>
	class StopwatchClock : public Clock {

	public:

	protected:

		Clock* source;


	private:



		

	};




}}



#endif