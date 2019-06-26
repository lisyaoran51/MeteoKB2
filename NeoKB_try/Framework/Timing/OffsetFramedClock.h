#ifndef OFFSET_FRAMED_CLOCK_H
#define OFFSET_FRAMED_CLOCK_H

#include "FrameBasedClock.h"
#include "FrameTimeInfo.h"
#include "FramedClock.h"

namespace Framework {
namespace Timing {

	/// <summary>
	/// �h�a�@�Ӧr�w��offset
	/// </summary>
	class OffsetFramedClock : public FramedClock {

	public:

	protected:

		Clock* source;


	private:

		double offset;

		

	};




}}



#endif