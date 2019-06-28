#ifndef OFFSET_CLOCK_H
#define OFFSET_CLOCK_H

#include "Clock.h"


namespace Framework {
namespace Timing {

	class OffsetClock : virtual public Clock {

	protected:

	public:

		virtual double GetOffset() = 0;
		virtual int SetOffset(double o) = 0;

	};




}}




#endif