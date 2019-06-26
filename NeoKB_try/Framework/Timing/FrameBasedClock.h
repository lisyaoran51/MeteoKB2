#ifndef FRAME_BASED_CLOCK_H
#define FRAME_BASED_CLOCK_H

#include "Clock.h"

#include "FrameTimeInfo.h"

namespace Framework {
namespace Timing {

	/// <summary>
	/// A clock which will only update its current time when a frame proces is triggered.
	/// Useful for keeping a consistent time state across an individual update.
	/// </summary>
	class FrameBasedClock : public Clock {


	protected:
		/// <summary>
		/// Elapsed time since last frame in milliseconds.
		/// </summary>
		double ElapsedFrameTime;

		double AverageFrameTime;
		double FramesPerSecond;

		FrameTimeInfo* TimeInfo;

	public:

		double GetCurrentTime();

		double GetElapsedFrameTime();

		double GetAverageFrameTime();

		double GetFramesPerSecond();

		FrameTimeInfo* GetFrameTimeInfo();

		/// <summary>
		/// Processes one frame. Generally should be run once per update loop.
		/// </summary>
		int ProcessFrame();


		

	};




}}



#endif