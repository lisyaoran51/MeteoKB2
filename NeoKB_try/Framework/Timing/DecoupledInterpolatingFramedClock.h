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
	/// 歌曲結束以後還可以繼續跑的clock，或是可以在歌曲開始前提早開始跑 ex歌曲剛開始就有一個音，就可以從-1秒開始跑
	///
	/// 這個裡面有5個clock，有InterpolatingFramedClock的source(可能會停掉)、framedSource(source的frame版)、interpolated clock(內插用)
	/// 還有自己的decoupled clock(分離的clock)、decoupled stopwatch(分離的clock的時鐘源)
	/// 這邊只要看自己的就好，先不用看InterpolatingFramedClock
	/// </summary>
	class DecoupledInterpolatingFramedClock : public InterpolatingFramedClock, public AdjustableClock {

	public:

		DecoupledInterpolatingFramedClock();
		virtual ~DecoupledInterpolatingFramedClock();

		// Clock
		virtual double GetCurrentTime();
		virtual int SetRate(double r);
		virtual int SetIsRunning(bool value);
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
		virtual int ResetSpeedAdjustments();
		


		// this
		int SetIsCoupled(bool value);
		bool GetIsCoupled();


	protected:


	private:

		bool isCoupled = true;

		AdjustableClock* getAdjustableSource();

		/// <summary>
		/// 要擺另一個clock，當source停住時就切換
		/// </summary>
		FramedClock* decoupledClock;

		StopwatchClock* decoupledStopwatchClock;

		/// <summary>
		/// 當分離時鐘和源實中沒有綁起來，且原時鐘也沒有在跑，才可以回true
		/// </summary>
		bool getIsUseDecoupledClock();

	};




}}



#endif