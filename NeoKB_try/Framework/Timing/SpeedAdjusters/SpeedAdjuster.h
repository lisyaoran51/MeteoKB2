#ifndef SPEED_ADJUSTER_H
#define SPEED_ADJUSTER_H

namespace Framework {
namespace Timing {
namespace SpeedAdjusters {

	class SpeedAdjuster {

		double seekTime;

		double accumulatedElapsedTime;

	public:
		SpeedAdjuster();
		
		virtual double GenerateTime(double elapsedTime) = 0;

		virtual int Reset();

		virtual int SetSeekTime(double sTime);

		virtual double GetSeekTime();
	};


}}}






#endif