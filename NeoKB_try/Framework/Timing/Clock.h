#ifndef CLOCK_H
#define CLOCK_H



namespace Framework {
namespace Timing {

	class Clock {

	protected:

		/// <summary>
		/// The current time of this clock, in milliseconds.
		/// </summary>
		//double currentTime;

		/// <summary>
		/// The rate this clock is running at, relative to real-time.
		/// </summary>
		//double rate;

		/// <summary>
		/// Whether this clock is currently running or not.
		/// </summary>
		//bool isRunning;

	public:

		virtual double GetCurrentTime() = 0;
		virtual int SetRate(double r) = 0;
		virtual double GetRate() = 0;
		virtual int SetIsRunning(bool value) = 0;
		virtual bool GetIsRunning() = 0;

	};




}}




#endif