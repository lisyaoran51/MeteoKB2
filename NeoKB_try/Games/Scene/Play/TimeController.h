#ifndef TIME_CONTROLLER_H
#define TIME_CONTROLLER_H

#include "../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../Framework/Timing/SpeedAdjusters/SpeedAdjuster.h"
#include "../../../Framework/Timing/DecoupledInterpolatingFramedClock.h"
#include "../MeteoScene.h"
#include "../../../Framework/Input/KeyBindings/KeyBindingHandler.h"


using namespace Framework::Timing::SpeedAdjusters;
using namespace Framework::Timing;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Input::KeyBindings;


namespace Games {
namespace Scenes {
namespace Play {

	/// <summary>
	/// ���o�Ӥ��Oscene�A���Ӳ���timing�h
	/// </summary>
	template<typename T>
	class TTimeController : public TimeController, public KeyBindingHandler<T> {

		int load() {

			// �ݭn��input key��binding�@�U�A�~���D���ӬOpause

			return 0;
		}

	public:

		TTimeController() {

			registerLoad(bind(static_cast<int(TTimeController<T>::*)(void)>(&TTimeController<T>::load), this));

		}

		virtual map<T, InputKey>* GetDefaultkeyBindings() = 0;

		virtual int OnKeyDown(pair<T, int> action) {
			return 0;
		}

		virtual int OnKeyUp(T action) {
			return 0;
		}

		virtual int OnButtonDown(T action) {
			if (keyBindings[action] == InputKey::Pause) {
				if (!GetIsPaused()) {
					Pause();
					SetAllChildsIsAvailableForTrigger(false);
				}
				else {
					// TODO: ���ӭn���]����Aresume�A���L�����g
					Resume();
					RecoverAllChildsIsAvailableForTrigger();
				}
			}
			return 0;
		}

		virtual int OnButtonUp(T action) {
			return 0;
		}

		virtual int OnKnobTurn(pair<T, int> action) {
			if (keyBindings[action.first] == InputKey::SpeedKnob) {
				SetRate(GetRate() + action.second);


			}
			if (keyBindings[action.first] == InputKey::SectionKnob) {

				JumpTo(sectionStartTime[getTempSection() + action.second]);

			}
			return 0;
		}

		virtual int OnSlide(pair<T, int> action) {
			return 0;
		}

	protected:

		map<T, InputKey> keyBindings;

		/// <summary>
		/// �bload�������ɭԡA�ɰO�n���檺�u�@
		/// </summary>
		virtual int LoadOnCompleted() {

			reloadMappings();

			return 0;
		}

		/// <summary>
		/// ��input key�M�s����J���X�@�U
		/// </summary>
		virtual int reloadMappings() = 0;


	};

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
	/// ���o�Ӥ��Oscene�A���Ӳ���timing�h
	/// </summary>
	class TimeController : public Container {


		int load();


		/// <summary>
		/// �i�H����q���������_����clock�A�N�O�q������clock�A�~���]�@���B�~������
		/// </summary>
		AdjustableClock* audioClock;

		/// <summary>
		/// framedClock��source�A�O�i�H�Ψӽվ�ɶ���
		/// </summary>
		DecoupledInterpolatingFramedClock* controllableClock;

		/// <summary>
		/// �@�ӻPparent�W�ߪ��ɲסA�U���������C��������A�C���ھڳo�Ӯ����B��
		/// </summary>
		FramedClock* framedClock;

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


	public:

		TimeController();

		/// <summary>
		/// pause container�������O�bplayer�̭��������A���Opause container�ۤv��
		/// </summary>
		int SetAudioClock(AdjustableClock* dInterpolatingFramedClock);

		int SetControllableClock(DecoupledInterpolatingFramedClock* cClock);

		int SetFramedClock(FramedClock* fClock);

		int SetSpeedAdjuster(SpeedAdjuster* sAdjuster);

		int JumpTo(double seekTime);

		int JumpToWithSpeedAdjust(double seekTime);

		int Pause();

		int Resume();

		int SetRate(double rate);

		double GetRate();

		bool GetIsPaused();


		int ImportWorkingSm(WorkingSm* workingSm);


	protected:


		/// <summary>
		/// �bupdate���ˬd���S���Q�Ȱ��A�����ܴN��ۤv��������
		/// </summary>
		virtual int update();

		WorkingSm* workingSm;

		int getTempSection();

		int getTempPart();

		vector<float> sectionStartTime;

		vector<float> partStartTime;

	private:

	};




}
}}







#endif