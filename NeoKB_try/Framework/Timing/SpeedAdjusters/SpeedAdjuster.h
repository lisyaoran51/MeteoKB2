#ifndef SPEED_ADJUSTER_H
#define SPEED_ADJUSTER_H

#include "../../Allocation/Hierachal/Container.h"

namespace Framework {
namespace Timing {
namespace SpeedAdjusters {

	enum class SpeedAdjusterState {
		None,
		Normal,
		FastForward,
		Backward,
		Freezing

	};

	class SpeedAdjuster : public Container{

	public:

		SpeedAdjuster();
		
		virtual int ProcessFrame(double elapsedTime) = 0;

		virtual int Reset() = 0;

		virtual int Pause();

		virtual int Resume();

		virtual SpeedAdjusterState GetSpeedAdjusterState() = 0;

		virtual int SetSeekTime(double sTime) = 0;

		virtual double GetSeekTime() = 0;

		/// <summary>
		/// 本次調整的時間，每次process frame就會更新一次，用來讓time controller得知這次需要調整多少時間
		///	</summary>
		virtual double GetAdjustFrameTime() = 0;

		virtual int SetFreezeTime(double fTime) = 0;

		virtual double GetFreezeTimeLeft() = 0;

		virtual bool GetIsAdjustingTime() = 0;

		/// <summary>
		/// 最後一次adjust time以後，會有一個time to adjust，然後seek time left匯市0，這個情況就是last adjusting time。下一次process frame以後這個狀況就會解除
		///	</summary>
		virtual bool GetIsLastAdjustingTime() = 0;

		virtual bool GetIsFreezingTime() = 0;

		/// <summary>
		/// 最後一次freeze time以後，然後freeze time left匯市0，這個情況就是last freezing time。下一次process frame以後這個狀況就會解除
		///	</summary>
		virtual bool GetIsLastFreezingTime() = 0;

		template<class _Type>
		int AddOnAdjustFreeze(_Type* callableObject, function<int()> callback, string name = "TriggerOnAdjust") {
			onAdjustFreeze.Add(callableObject, callback, name);
			return 0;
		}

		
		template<class _Type>
		int AddOnAdjustFreezeEnd(_Type* callableObject, function<int()> callback, string name = "TriggerOnAdjustEnd") {
			onAdjustFreezeEnd.Add(callableObject, callback, name);
			return 0;
		}

	protected:

		ActionList<int> onAdjustFreeze;

		/// <summary>
		/// 再adjust或freeze結束時發動，如果先pause再adjust，則他呼叫resume會沒有效果，直到再按一次pause才行resume
		/// </summary>
		ActionList<int> onAdjustFreezeEnd;

	};


}}}






#endif