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

		FramedClock(Clock* s = NULL);

		Clock* GetSource();

		double GetCurrentTime();

		int ProcessFrame();

	protected:

		Clock* source;


	private:


		double lastFrameTime;

		double sourceTime;

		bool processSourceClockFrames;

		double timeUntilNextCalculation;
		double timeSinceLastCalculation;
		int framesSinceLastCalculation;

		

	};




}}



#endif