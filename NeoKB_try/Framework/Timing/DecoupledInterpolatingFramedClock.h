#ifndef DECOUPLED_INTERPOLATING_FRAMED_CLOCK_H
#define DECOUPLED_INTERPOLATING_FRAMED_CLOCK_H

#include "FrameBasedClock.h"
#include "FrameTimeInfo.h"
#include "InterpolatingFramedClock.h"

namespace Framework {
namespace Timing {

	/// <summary>
	/// Adds the ability to keep the clock running even when the underlying source has stopped or cannot handle the current time range.
	/// This is handled by performing seeks on the underlying source and checking whether they were successful or not.
	/// On failure to seek, we take over with an internal clock until control can be returned to the actual source.
	///
	/// This clock type removes the requirement of having a source set.
	/// </summary>
	class DecoupledInterpolatingFramedClock : public InterpolatingFramedClock {

	public:

		DecoupledInterpolatingFramedClock(Clock* s = NULL);

		double GetCurrentTime();

		Clock* GetSource();

		int ProcessFrame();

		int ChangeSource(Clock* s);

	protected:

		Clock* source;


	private:


		/// <summary>
		/// 要擺另一個clock，當source停住時就切換
		/// </summary>
		FramedClock* decoupledClock;

		

	};




}}



#endif