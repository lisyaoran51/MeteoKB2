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
	/// �q�������H���٥i�H�~��]��clock�A�άO�i�H�b�q���}�l�e�����}�l�] ex�q����}�l�N���@�ӭ��A�N�i�H�q-1��}�l�]
	///
	/// �o�Ӹ̭���5��clock�A��InterpolatingFramedClock��source(�i��|����)�BframedSource(source��frame��)�Binterpolated clock(������)
	/// �٦��ۤv��decoupled clock(������clock)�Bdecoupled stopwatch(������clock��������)
	/// �o��u�n�ݦۤv���N�n�A�����ά�InterpolatingFramedClock
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
		/// �n�\�t�@��clock�A��source����ɴN����
		/// </summary>
		FramedClock* decoupledClock;

		StopwatchClock* decoupledStopwatchClock;

		/// <summary>
		/// ����������M���ꤤ�S���j�_�ӡA�B������]�S���b�]�A�~�i�H�^true
		/// </summary>
		bool getIsUseDecoupledClock();

	};




}}



#endif