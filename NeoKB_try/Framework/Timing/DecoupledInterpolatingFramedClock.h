#ifndef DECOUPLED_INTERPOLATING_FRAMED_CLOCK_H
#define DECOUPLED_INTERPOLATING_FRAMED_CLOCK_H

#include "FrameBasedClock.h"
#include "FrameTimeInfo.h"
#include "InterpolatingFramedClock.h"
#include "AdjustableClock.h"
#include "StopwatchClock.h"

namespace Framework {
namespace Timing {

	/// <summary>
	/// Adds the ability to keep the clock running even when the underlying source has stopped or cannot handle the current time range.
	/// This is handled by performing seeks on the underlying source and checking whether they were successful or not.
	/// On failure to seek, we take over with an internal clock until control can be returned to the actual source.
	///
	/// This clock type removes the requirement of having a source set.
	/// </summary>
	class DecoupledInterpolatingFramedClock : public InterpolatingFramedClock, public AdjustableClock {

	public:

		DecoupledInterpolatingFramedClock();

		// Clock
		virtual double GetCurrentTime();
		virtual int SetRate(double r);
		virtual double GetRate();
		virtual bool GetIsRunning();

		// FrameBasedClock
		virtual int ChangeSource(Clock* s);
		virtual double GetElapsedFrameTime();
		virtual int ProcessFrame();

		// AdjustableClock
		virtual int Reset();
		virtual int Start();
		virtual int Stop();
		virtual bool Seek(double position);
		virtual int ResetAdjustments();


		// this
		int SetIsCoupled(bool value);
		bool GetIsCoupled();


	protected:


	private:

		bool isCoupled;

		AdjustableClock* adjustableSource;

		/// <summary>
		/// 要擺另一個clock，當source停住時就切換
		/// </summary>
		FramedClock* decoupledClock;

		StopwatchClock* decoupledStopwatchClock;

		bool getIsUseDecoupledClock();

	};




}}



#endif