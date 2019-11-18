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
		virtual double GetCurrentTime();
		virtual int SetIsRunning(bool value);
		virtual bool GetIsRunning();

		virtual double GetLastFrameTime();
		virtual double GetElapsedFrameTime();
		virtual double GetAverageFrameTime();
		virtual double GetFramesPerSecond();
		virtual FrameTimeInfo* GetFrameTimeInfo();
		virtual int SetIsProcessSourceClockFrames(bool value);
		virtual bool GetIsProcessSourceClockFrames();

	protected:

		Clock* source;

		double lastFrameTime;

		virtual int setCurrentTime(double cTime);

		/// <summary>
		/// 因為set current time會把last frame time更新，而且會去seek他的source的時間，是用來給外面程式調整時間用，不適內部設定用
		/// 這個是單純設定內部的current time
		/// </summary>
		virtual int setInnerCurrentTime(double cTime);

		virtual int start();

	private:

		double currentTime;
		double rate;
		bool isStarted = false;
		bool isRunning = false;

		double averageFrameTime;
		double framesPerSecond;
		double timeUntilNextCalculation;
		double timeSinceLastCalculation;
		int framesSinceLastCalculation;


		double sourceTime;

		bool isProcessSourceClockFrames;

	};




}}



#endif