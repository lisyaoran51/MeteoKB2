#ifndef TIME_CONTROLLER_H
#define TIME_CONTROLLER_H

#include "../../../Framework/Scenes/Scene.h"
#include "../../../Framework/Timing/SpeedAdjusters/SpeedAdjuster.h"
#include "../../../Framework/Timing/DecoupledInterpolatingFramedClock.h"

using namespace Base::Scenes;
using namespace Framework::Timing::SpeedAdjusters;
using namespace Framework::Timing;


namespace Base {
namespace Scenes {
namespace Play {

	/// <summary>
	/// �\�bplayer�U���A�Ψӱ���C���t�שM�Ȱ��B���p�`
	///	</summary>
	/// <summary>
	/// osu�����S���Ȱ���A�Ȱ��N�n�Y�p�����A�ҥH�S���H�|�h����pause container�A�ӬOpause container�ۤv�h�߰�host�O�_active�A�p�G
	/// �����Y�p�~�|pause
	/// 
	/// �����k���ӬO �Aplayfield load���ɭԡA��Event processor��algorithm add child�A���o��algorithm��pause container�����
	/// �M��bplayfield add event processor���ɭԡA�A��algorithm�\�J�o��processor�̡C
	///
	/// pause container�������O�bplayer�̭��������A���Opause container�ۤv��
	/// </summary>
	class TimeController : public Scene {
	public:

		/// <summary>
		/// pause container�������O�bplayer�̭��������A���Opause container�ۤv��
		/// </summary>
		int SetDecoupledInterpolatingFramedClock(DecoupledInterpolatingFramedClock* dInterpolatingFramedClock);

		int SetAdjustableClock(AdjustableClock* aClock);

		int SetSpeedAdjuster(SpeedAdjuster* sAdjuster);

		int JumpTo(double seekTime);

		int JumpToWithSpeedAdjust(double seekTime);

		int Pause();

		int Resume();

		int SetRate(double rate);

		int GetRate();


	protected:


		/// <summary>
		/// �bupdate���ˬd���S���Q�Ȱ��A�����ܴN��ۤv��������
		/// </summary>
		virtual int update();

	private:

		int load();


		/// <summary>
		/// �@�ӻPparent�W�ߪ��ɲסA�U���������C��������A�C���ھڳo�Ӯ����B��
		/// </summary>
		DecoupledInterpolatingFramedClock* controllableClock;

		AdjustableClock* audioClock;

		SpeedAdjuster* speedAdjuster;

		double rate;

		bool isTriggeredSeekingTime;
		bool isSeekingTime;
		double targetSeekTime;

		bool isTriggeredPause;
		bool isPaused;

		bool isTriggeredResume;

		bool isControllable;

		bool isPausable;
	};




}}}







#endif