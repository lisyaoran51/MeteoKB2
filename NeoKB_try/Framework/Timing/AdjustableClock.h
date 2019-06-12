#ifndef ADJUSTABLE_CLOCK_H
#define ADJUSTABLE_CLOCK_H



namespace Framework {
namespace Timing {

	class AdjustableClock {

	public:

		/// <summary>
		/// Stop and reset position.
		/// </summary>
		virtual int Reset() = 0;

		/// <summary>
		/// Start (resume) running.
		/// </summary>
		virtual int Start() = 0;

		/// <summary>
		/// Stop (pause) running.
		/// </summary>
		virtual int Stop() = 0;

		/// <summary>
		/// Seek to a specific time position.
		/// </summary>
		/// <returns>Whether a seek was possible.</returns>
		virtual bool Seek(double position) = 0;

		/// <summary>
		/// The rate this clock is running at, relative to real-time.
		/// </summary>
		double Rate;

		/// <summary>
		/// Reset the rate to a stable value.
		/// </summary>
		virtual int ResetSpeedAdjustments() = 0;



	};




}}




#endif