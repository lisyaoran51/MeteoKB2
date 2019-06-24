#ifndef FRAMED_CLOCK_H
#define FRAMED_CLOCK_H

#include "FrameBasedClock.h"
#include "FrameTimeInfo.h"

namespace Framework {
namespace Timing {

	/// <summary>
	/// Takes a clock source and separates time reading on a per-frame level.
	/// The CurrentTime value will only change on initial construction and whenever ProcessFrame is run.
	/// </summary>
	class FramedClock : public FrameBasedClock {

	public:

		double LastFrameTime;

		double SourceTime;

		bool ProcessSourceClockFrames;

		int ProcessFrame();

	private:

		Clock* source;

		double timeUntilNextCalculation;
		double timeSinceLastCalculation;
		int framesSinceLastCalculation;

		

	};




}}



#endif