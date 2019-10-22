#ifndef OFFSET_FRAMED_CLOCK_H
#define OFFSET_FRAMED_CLOCK_H

#include "FrameBasedClock.h"
#include "FrameTimeInfo.h"
#include "FramedClock.h"
#include "OffsetClock.h"

namespace Framework {
namespace Timing {

	/// <summary>
	/// 多家一個字定的offset
	/// </summary>
	class OffsetFramedClock : public FramedClock, public OffsetClock {

	public:

		OffsetFramedClock(Clock* s);

		virtual double GetCurrentTime();

		int SetOffset(double o);
		double GetOffset();

	protected:

	private:

		double offset;

		

	};




}}



#endif