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
	class InterpolatingFramedClock : public FrameBasedClock {

	public:

		InterpolatingFramedClock(Clock* s = nullptr);

		virtual int ProcessFrame();

		virtual int ChangeSource(Clock* s);


		virtual double GetCurrentTime();
		virtual int SetRate(double r);
		virtual double GetRate();
		virtual int SetIsRunning(bool value);
		virtual bool GetIsRunning();

		virtual double GetDrift();

		virtual double GetElapsedFrameTime();
		virtual double GetAverageFrameTime();
		virtual double GetFramesPerSecond();

		/// <summary>
		/// new一個timeinfo給你，用完要記得回收
		/// </summary>
		virtual FrameTimeInfo* GetFrameTimeInfo();

	protected:

		Clock* source = nullptr;

		double rate = 1.0;

		/// <summary>
		/// 把source外面包一層frame
		/// </summary>
		FrameBasedClock* framedSource = nullptr;



	private:


		/// <summary>
		/// 要擺一個stopwatch當他的source
		/// 用來把目前的source再內切一下，避免source跟實際時間落差太大，會有lag
		/// </summary>
		FramedClock* interpolateClock;

		bool sourceIsRunning = false;

		double lastInterpolatedTime = 0;

		double currentInterpolatedTime = 0;

		double allowableErrorInSeconds;

	};




}}



#endif