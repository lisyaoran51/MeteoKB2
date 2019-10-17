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
	/// 其實這個不是scene，應該移到timing去
	/// </summary>
	template<typename T>
	class TTimeController : public TimeController, public KeyBindingHandler<T> {

		int load() {

			// 需要跟input key做binding一下，才知道哪個是pause

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
					// TODO: 應該要等跑完圈再resume，不過先不寫
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
		/// 在load結束的時候，時記要執行的工作
		/// </summary>
		virtual int LoadOnCompleted() {

			reloadMappings();

			return 0;
		}

		/// <summary>
		/// 把input key和新的輸入結合一下
		/// </summary>
		virtual int reloadMappings() = 0;


	};

	/// <summary>
	/// 擺在player下面，用來控制遊戲速度和暫停、跳小節
	///	</summary>
	/// <summary>
	/// osu本身沒有暫停鍵，暫停就要縮小視窗，所以沒有人會去控制pause container，而是pause container自己去詢問host是否active，如果
	/// 視窗縮小才會pause
	/// 
	/// 控制的方法應該是 再playfield load的時候，把Event processor的algorithm add child，讓這個algorithm有pause container的資料
	/// 然後在playfield add event processor的時候，再把algorithm擺入這些processor裡。
	///
	/// pause container的時鐘是在player裡面指派的，不是pause container自己的
	/// 其實這個不是scene，應該移到timing去
	/// </summary>
	class TimeController : public Container {


		int load();


		/// <summary>
		/// 可以跳到歌曲的任何斷落的clock，就是歌曲中的clock，外面包一個額外的時鐘
		/// </summary>
		AdjustableClock* audioClock;

		/// <summary>
		/// framedClock的source，是可以用來調整時間的
		/// </summary>
		DecoupledInterpolatingFramedClock* controllableClock;

		/// <summary>
		/// 一個與parent獨立的時終，下面接的式遊戲的物件，遊戲根據這個時鐘運行
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
		/// pause container的時鐘是在player裡面指派的，不是pause container自己的
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
		/// 在update時檢查有沒有被暫停，有的話就把自己時鐘停掉
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