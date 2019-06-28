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
	class FramedClock : virtual public FrameBasedClock {

	public:

		FramedClock(Clock* s = nullptr);

		virtual int ProcessFrame();

		virtual Clock* GetSource();
		virtual int SetRate(double r);
		virtual double GetRate();
		// protected SetCurrentTime();
		virtual double GetCurrentTime();
		virtual int SetIsRunning(bool value);
		virtual bool GetIsRunning();

		virtual double GetElapsedFrameTime();
		virtual double GetAverageFrameTime();
		virtual double GetFramesPerSecond();
		virtual FrameTimeInfo* GetFrameTimeInfo();
		virtual int SetIsProcessSourceClockFrames(bool value);
		virtual bool GetIsProcessSourceClockFrames();

	protected:

		Clock* source;

		virtual int setCurrentTime(double cTime);

	private:

		double currentTime;
		double rate;
		bool isRunning;

		double averageFrameTime;
		double framesPerSecond;
		double timeUntilNextCalculation;
		double timeSinceLastCalculation;
		int framesSinceLastCalculation;

		double lastFrameTime;

		double sourceTime;

		bool isProcessSourceClockFrames;

	};




}}



#endif