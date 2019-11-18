#ifndef STOPWATCH_CLOCK_H
#define STOPWATCH_CLOCK_H

#include "FrameBasedClock.h"
#include "FrameTimeInfo.h"
#include "FramedClock.h"
#include "AdjustableClock.h"
#include <chrono>



using namespace std::chrono;


namespace Framework {
namespace Timing {

	/// <summary>
	/// �̭����@�ӽX���source�A�i�H�}��
	/// </summary>
	class StopwatchClock : virtual public AdjustableClock {

	public:

		StopwatchClock();

		virtual double GetCurrentTime();
		virtual int SetRate(double r);
		virtual double GetRate();
		virtual int SetIsRunning(bool value);
		virtual bool GetIsRunning();


		/// <summary>
		/// ���o�p�ɾ�������ثe��׼ơC
		/// </summary>
		//static long GetTimestamp();

		/// <summary>
		/// ����ɶ����j���q�ñN�w�ӥήɶ����]���s�C
		/// </summary>
		virtual int Reset();

		/// <summary>
		/// ����ɶ����j���q�A�ñN�ӥήɶ����]���s�A�M��}�l���q�ӥήɶ��C
		/// </summary>
		virtual int Restart();

		/// <summary>
		/// �Ұʩ��~����q���j���w�ӥήɶ��C
		/// </summary>
		virtual int Start();

		/// <summary>
		/// ������q���j���w�ӥήɶ��C
		/// </summary>
		virtual int Stop();

		/// <summary>
		/// Seek to a specific time position.
		/// </summary>
		/// <returns>Whether a seek was possible.</returns>
		virtual bool Seek(double position);

		/// <summary>
		/// Reset the rate to a stable value.
		/// </summary>
		virtual int ResetSpeedAdjustments();

	protected:

		/// <summary>
		/// ���o�ثe�������Ҵ��q���w�ӥήɶ��`�M�C
		/// </summary>
		//TimeSpan elapsed;

		/// <summary>
		/// ���o�ثe�������Ҵ��q���w�ӥήɶ��`�M�C
		/// </summary>
		//long elapsedMilliseconds;

		/// <summary>
		/// ���o�ثe�������Ҵ��q���w�ӥήɶ��`�M�C
		/// </summary>
		//long elapsedTicks;

	private:

		double rate = 1.0;

		bool isRunning;

		bool isStarted;

		system_clock::time_point systemCurrentTime;

		system_clock::time_point systemStartTime;

		double rateChangeAccumulatedTime;

		double seekOffset;

		/// <summary>
		/// ���o�ثe�������Ҵ��q���w�ӥήɶ��`�M�C
		/// </summary>
		long long getElapsedMicroseconds();

		double getElapsedSeconds();



	};




}}



#endif