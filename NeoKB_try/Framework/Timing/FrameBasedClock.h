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
	class FrameBasedClock : virtual public Clock {



	protected:

		//double averageFrameTime;
		//double framesPerSecond;

	public:

		/// <summary>
		/// Elapsed time since last frame in milliseconds.
		/// </summary>
		virtual double GetElapsedFrameTime() = 0;

		virtual double GetAverageFrameTime() = 0;

		virtual double GetFramesPerSecond() = 0;

		/// <summary>
		/// new一個timeinfo給你，用完要記得回收
		/// </summary>
		virtual FrameTimeInfo* GetFrameTimeInfo() = 0;

		/// <summary>
		/// Processes one frame. Generally should be run once per update loop.
		/// </summary>
		virtual int ProcessFrame() = 0;


		

	};




}}



#endif