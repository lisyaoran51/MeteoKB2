#ifndef SPEED_ADJUSTER_H
#define SPEED_ADJUSTER_H

#include "../../Allocation/Hierachal/Container.h"

namespace Framework {
namespace Timing {
namespace SpeedAdjusters {

	class SpeedAdjuster : public Container{

	public:

		SpeedAdjuster();
		
		virtual int ProcessFrame(double elapsedTime) = 0;

		virtual int Reset() = 0;

		virtual int SetSeekTime(double sTime) = 0;

		virtual double GetSeekTime() = 0;

		/// <summary>
		/// �����վ㪺�ɶ��A�C��process frame�N�|��s�@���A�Ψ���time controller�o���o���ݭn�վ�h�֮ɶ�
		///	</summary>
		virtual double GetAdjustFrameTime() = 0;

		virtual int SetFreezeTime(double fTime) = 0;

		virtual double GetFreezeTimeLeft() = 0;

		virtual bool GetIsAdjustingTime() = 0;

		virtual bool GetIsFreezingTime() = 0;

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
		/// �Aadjust��freeze�����ɵo�ʡA�p�G��pause�Aadjust�A�h�L�I�sresume�|�S���ĪG�A����A���@��pause�~��resume
		/// </summary>
		ActionList<int> onAdjustFreezeEnd;

	};


}}}






#endif