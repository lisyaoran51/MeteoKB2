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
		/// �����վ㪺�ɶ��A�C��process frame�N�|��s�@���A�Ψ���time controller�o���o���ݭn�վ�h�֮ɶ�
		///	</summary>
		virtual double GetAdjustFrameTime() = 0;

		virtual int SetFreezeTime(double fTime) = 0;

		virtual double GetFreezeTimeLeft() = 0;

		virtual bool GetIsAdjustingTime() = 0;

		/// <summary>
		/// �̫�@��adjust time�H��A�|���@��time to adjust�A�M��seek time left�ץ�0�A�o�ӱ��p�N�Olast adjusting time�C�U�@��process frame�H��o�Ӫ��p�N�|�Ѱ�
		///	</summary>
		virtual bool GetIsLastAdjustingTime() = 0;

		virtual bool GetIsFreezingTime() = 0;

		/// <summary>
		/// �̫�@��freeze time�H��A�M��freeze time left�ץ�0�A�o�ӱ��p�N�Olast freezing time�C�U�@��process frame�H��o�Ӫ��p�N�|�Ѱ�
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
		/// �Aadjust��freeze�����ɵo�ʡA�p�G��pause�Aadjust�A�h�L�I�sresume�|�S���ĪG�A����A���@��pause�~��resume
		/// </summary>
		ActionList<int> onAdjustFreezeEnd;

	};


}}}






#endif