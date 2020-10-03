#ifndef TIME_CONTROLLER_H
#define TIME_CONTROLLER_H

#include "../Allocation/Hierachal/Container.h"
#include "SpeedAdjusters/SpeedAdjuster.h"
#include "DecoupledInterpolatingFramedClock.h"
#include "../Input/KeyBindings/KeyBindingHandler.h"
#include "../Input/Commands/CommandHandler.h"


using namespace Framework::Timing::SpeedAdjusters;
using namespace Framework::Timing;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Input::KeyBindings;
using namespace Framework::Input::Commands;



namespace Framework {
namespace Timing {

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

	public:

		TimeController();
		~TimeController();

		/// <summary>
		/// 這個時鐘的時鐘源就是audio clock，不過他可以跳到audio範圍以外的時間
		/// </summary>
		int SetControllableClock(AdjustableClock* aClock);

		AdjustableClock* GetControllableClock();

		int SetSpeedAdjuster(SpeedAdjuster* sAdjuster);

		int JumpTo(double seekTime);

		int JumpToWithSpeedAdjust(double seekTime);

		int Pause();

		/// <summary>
		/// 這邊要判斷如果是在adjust之前就已經pause了，那resume就會失敗。會等到pause結束才會成公
		/// </summary>
		int Resume();

		int SetRate(double rate);

		double GetRate();

		bool GetIsPaused();

		int SetIsAllowSeek(bool iAllowSeek);

		bool GetIsAllowSeek();

		template<class _Type>
		int AddOnRetry(_Type* callableObject, function<int()> callback, string name = "HandleRetryRequest") {
			onRetryRequested.Add(callableObject, callback, name);
			return 0;
		}

		template<class _Type>
		int AddOnQuit(_Type* callableObject, function<int()> callback, string name = "HandleQuitRequest") {
			onQuitRequested.Add(callableObject, callback, name);
			return 0;
		}

		/* 暫時不寫這段，以後響到要怎麼寫再回來改
		int ImportWorkingSm(WorkingSm* workingSm);
		*/

	protected:


		/// <summary>
		/// 在update時檢查有沒有被暫停，有的話就把自己時鐘停掉
		/// </summary>
		virtual int update();

		/// <summary>
		/// 在調整時間的時候，用來做初速度效果的東西
		/// </summary>
		SpeedAdjuster* speedAdjuster = nullptr;

		/// <summary>
		/// 可以跳到歌曲的任何斷落的clock，就是歌曲中的clock，外面包一個額外的時鐘
		/// </summary>
		AdjustableClock* controllableClock = nullptr;

		/// <summary>
		/// 一個與parent獨立的時終，下面接的式遊戲的物件，遊戲根據這個時鐘運行
		/// </summary>
		//FramedClock* gameClock;

		bool isAllowSeek = true;

		double rate = 1.0;

		double targetSeekTime;

		double defaultFreezeTime = 1.0;
		double defaultAdjustTime = 5.0;

		bool isPaused = false;
		bool isAdjustAfterPause = false;

		/// <summary>
		/// 當暫停結束，要繼續遊戲時，有個倒數時間，這時isWaitingFreeze就會是true
		/// </summary>
		bool isWaitingFreeze = false;

		ActionList<int> onRetryRequested;
		ActionList<int> onQuitRequested;


		/* 暫時不寫這段，以後響到要怎麼寫再回來改

		WorkingSm* workingSm;

		int getTempSection();

		int getTempPart();

		vector<float> sectionStartTime;

		vector<float> partStartTime;

		*/
	private:

	};

	/// <summary>
	/// 其實這個不是scene，應該移到timing去
	/// </summary>
	template<typename T, typename TCommand>
	class TTimeController : public TimeController, public KeyBindingHandler<T>, public CommandHandler<TCommand> {

		int load() {

			// 需要跟input key做binding一下，才知道哪個是pause
			// 應該是不用bind，input manager都bind好了，我們直接用就好


			return 0;
		}

	public:

		TTimeController(): RegisterType("TTimeController") {

			registerLoad(bind(static_cast<int(TTimeController<T, TCommand>::*)(void)>(&TTimeController<T>::load), this));

		}

		//virtual map<T, InputKey>* GetDefaultkeyBindings() = 0;

		virtual int OnKeyDown(pair<T, int> action) {
			return 0;
		}

		virtual int OnKeyUp(T action) {
			return 0;
		}

		/// <summary>
		/// 這段寫錯了，不要用
		/// </summary>
		virtual int OnButtonDown(T action) {
			typename map<T, InputKey>::iterator it;
			it = keyBindings.find(action);

			LOG(LogLevel::Debug) << "TTimeController::OnButtonDown(): check bindings " << (keyBindings.find(action) == keyBindings.end() ? "not found" : "found");
			if (keyBindings[action] == InputKey::Pause) {
				LOG(LogLevel::Debug) << "TTimeController::OnButtonDown() : get pause button input and pause.";
				if (speedAdjuster->GetIsAdjustingTime())
					return -1;

				if (!GetIsPaused()) {
					Pause();
					SetAllChildsIsMaskedForTrigger();
				}
				else if(!isWaitingFreeze){
					speedAdjuster->SetFreezeTime(defaultFreezeTime);
					isWaitingFreeze = true;
				}
			}
			return 0;
		}

		virtual int OnButtonUp(T action) {
			return 0;
		}

		/// <summary>
		/// 這段寫錯了，不要用
		/// </summary>
		virtual int OnKnobTurn(pair<T, int> action) {
			if (keyBindings[action.first] == InputKey::SpeedKnob) {
				SetRate(GetRate() + action.second);


			}
			if (keyBindings[action.first] == InputKey::SectionKnob) {

				if (isWaitingFreeze)
					return 0;

				//JumpTo(sectionStartTime[getTempSection() + action.second]);
				if (isPaused)
					isAdjustAfterPause = true;
				speedAdjuster->SetSeekTime(action.second * defaultAdjustTime);


			}
			return 0;
		}

		virtual int OnSlide(pair<T, int> action) {
			return 0;
		}

		virtual int OnCommand(TCommand* command) {
			return 0;
		}

	protected:


		map<T, InputKey> keyBindings;

		/// <summary>
		/// 在load結束的時候，時記要執行的工作
		/// </summary>
		virtual int LoadOnComplete() {

			//reloadMappings();

			return 0;
		}

		/// <summary>
		/// 把input key和新的輸入結合一下
		/// </summary>
		//virtual int reloadMappings() = 0;


	};

	





}}







#endif