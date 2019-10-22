#ifndef LINEAR_SPEED_ADJUSTER_H
#define LINEAR_SPEED_ADJUSTER_H

#include "SpeedAdjuster.h"


namespace Framework {
namespace Timing {
namespace SpeedAdjusters {

	/// <summary>
	/// �u�ʽվ�t��
	///	</summary>
	class LinearSpeedAdjuster : public SpeedAdjuster {

	public:


		/* Speed Adjuster */
		LinearSpeedAdjuster();

		virtual int ProcessFrame(double elapsedTime);

		virtual int Reset();

		virtual int SetSeekTime(double sTime);

		virtual double GetSeekTime();

		virtual double GetAdjustFrameTime();

		virtual int SetFreezeTime(double fTime);

		virtual double GetFreezeTimeLeft();

		virtual bool GetIsAdjustingTime();

		virtual bool GetIsFreezingTime();


		/* this */

		int SetAdjustSpeed(double aSpeed);

	protected:

		bool isAdjustingTime = false;

		bool isFreezingTime = false;

		double seekTimeLeft = 0.0;

		double freezeTimeLeft = 0.0;

		double adjustFrameTime = 0.0;

		/// <summary>
		/// 20���վ�|�b1�����A1:20
		///	</summary>
		double adjustSpeed = 20.0;

	};


}}}







#endif