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
		/// new�@��timeinfo���A�A�Χ��n�O�o�^��
		/// </summary>
		virtual FrameTimeInfo* GetFrameTimeInfo();

	protected:

		Clock* source = nullptr;

		double rate = 1.0;

		/// <summary>
		/// ��source�~���]�@�hframe
		/// </summary>
		FrameBasedClock* framedSource = nullptr;



	private:


		/// <summary>
		/// �n�\�@��stopwatch��L��source
		/// �Ψӧ�ثe��source�A�����@�U�A�קKsource���ڮɶ����t�Ӥj�A�|��lag
		/// </summary>
		FramedClock* interpolateClock;

		bool sourceIsRunning = false;

		double lastInterpolatedTime = 0;

		double currentInterpolatedTime = 0;

		double allowableErrorInSeconds;

	};




}}



#endif