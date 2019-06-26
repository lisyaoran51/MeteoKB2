#ifndef INTERPOLATING_FRAMED_CLOCK_H
#define INTERPOLATING_FRAMED_CLOCK_H

#include "FrameBasedClock.h"
#include "FrameTimeInfo.h"
#include "FramedClock.h"

namespace Framework {
namespace Timing {

	/// <summary>
	/// A clock which uses an internal stopwatch to interpolate (smooth out) a source.
	/// Note that this will NOT function unless a source has been set.
	/// </summary>
	class InterpolatingFramedClock : public FramedClock {

	public:

		InterpolatingFramedClock(Clock* s = NULL);

		double GetCurrentTime();

		Clock* GetSource();

		int ProcessFrame();

		int ChangeSource(Clock* s);

	protected:

		Clock* source;


	private:


		/// <summary>
		/// 要擺一個stopwatch當他的source
		/// </summary>
		FramedClock* interpolateClock;

		

	};




}}



#endif