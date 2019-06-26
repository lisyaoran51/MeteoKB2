#ifndef CLOCK_H
#define CLOCK_H



namespace Framework {
namespace Timing {

	class Clock {

		/// <summary>
		/// The current time of this clock, in milliseconds.
		/// </summary>
		double currentTime;

		/// <summary>
		/// The rate this clock is running at, relative to real-time.
		/// </summary>
		double rate;

		/// <summary>
		/// Whether this clock is currently running or not.
		/// </summary>
		bool isRunning;

	public:

		double GetCurrentTime();

	};




}}




#endif