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
	class ThrottledFramedClock : public FramedClock {

		double maxUpdateHz = 1000;

		/// <summary>
		/// 累積誤差，以ms為單位
		/// </summary>
		double accumulatedSleepError = 0;

		/// <summary>
		/// 如果跑太快，就讓tjread稅一下
		/// </summary>
		int throttle();

	public:

		ThrottledFramedClock(Clock* s = nullptr);

		ThrottledFramedClock(double mUpdateHz, Clock* s = nullptr);

		virtual int ProcessFrame();

		int SetMaxUpdateHz(double hz);

		double GetMaxUpdateHz();

	protected:

		double getThrottledElapsedFrameTime();


	};




}}



#endif