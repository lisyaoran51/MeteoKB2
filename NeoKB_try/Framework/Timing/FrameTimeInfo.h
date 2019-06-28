#ifndef FRAME_TIME_INFO_H
#define FRAME_TIME_INFO_H



namespace Framework {
namespace Timing {

	struct FrameTimeInfo {

	public:

		/// <summary>
		/// Elapsed time during last frame in milliseconds.
		/// </summary>
		double Elapsed;

		/// <summary>
		/// Begin time of this frame.
		/// </summary>
		double Current;

	};




}}




#endif