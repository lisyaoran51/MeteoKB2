#ifndef CLOCK_H
#define CLOCK_H



namespace Framework {
namespace Timing {

	class Clock {

	public:

		/// <summary>
		/// The current time of this clock, in milliseconds.
		/// </summary>
		double CurrentTime;

		

		/// <summary>
		/// The rate this clock is running at, relative to real-time.
		/// </summary>
		double Rate;


		/// <summary>
		/// Whether this clock is currently running or not.
		/// </summary>
		bool IsRunning;

	};




}}




#endif