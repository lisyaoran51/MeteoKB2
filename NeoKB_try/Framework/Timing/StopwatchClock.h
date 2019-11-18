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
	/// 裡面有一個碼表當做source，可以開關
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
		/// 取得計時器機制中的目前刻度數。
		/// </summary>
		//static long GetTimestamp();

		/// <summary>
		/// 停止時間間隔測量並將已耗用時間重設為零。
		/// </summary>
		virtual int Reset();

		/// <summary>
		/// 停止時間間隔測量，並將耗用時間重設為零，然後開始測量耗用時間。
		/// </summary>
		virtual int Restart();

		/// <summary>
		/// 啟動或繼續測量間隔的已耗用時間。
		/// </summary>
		virtual int Start();

		/// <summary>
		/// 停止測量間隔的已耗用時間。
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
		/// 取得目前執行個體所測量的已耗用時間總和。
		/// </summary>
		//TimeSpan elapsed;

		/// <summary>
		/// 取得目前執行個體所測量的已耗用時間總和。
		/// </summary>
		//long elapsedMilliseconds;

		/// <summary>
		/// 取得目前執行個體所測量的已耗用時間總和。
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
		/// 取得目前執行個體所測量的已耗用時間總和。
		/// </summary>
		long long getElapsedMicroseconds();

		double getElapsedSeconds();



	};




}}



#endif