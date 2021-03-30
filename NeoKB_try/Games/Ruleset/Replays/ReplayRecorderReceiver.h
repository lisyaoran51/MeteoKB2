#ifndef REPLAY_RECORDER_RECEIVER_H
#define REPLAY_RECORDER_RECEIVER_H


#include "../../../Framework/Allocation/Hierachal/Container.h"
#include "ReplayRecorder.h"
#include "../../../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../../../Framework/Timing/TimeController.h"


using namespace Framework::Allocation::Hierachal;
using namespace Framework::Input::KeyBindings;
using namespace Framework::Timing;


namespace Games {
namespace Rulesets {
namespace Replays {

	/// <summary>
	/// 因為replay recorder在ruleset input manager上面，所以抓步道ruleset的input，要把這個receiver百在playfield下面，才有辦法去抓input，然後再把input傳到ReplayRecorder去
	/// </summary>
	class ReplayRecorderReceiver : public Container {

		int load();

		int load(ReplayRecorder* r, TimeController* t);

	public:


		ReplayRecorderReceiver();


	protected:

		ReplayRecorder* replayRecorder = nullptr;

		/// <summary>
		/// 用來判斷是不是錄音中，如果是false就停止紀錄
		/// </summary>
		bool isRecording = true;

	};

	template<typename T>
	class TReplayRecorderReceiver : public ReplayRecorderReceiver, public KeyBindingHandler<T> {

	public:

		TReplayRecorderReceiver() :RegisterType("TReplayRecorderReceiver") {
		}

		virtual int OnKeyDown(pair<T, int> action) {
			if (!isRecording)
				return -1;

			if (replayRecorder != nullptr) {
				if (dynamic_cast<TReplayRecorder<T>*>(replayRecorder) != nullptr) {
					dynamic_cast<TReplayRecorder<T>*>(replayRecorder)->OnKeyDown(action);
					return 0;
				}
				LOG(LogLevel::Error) << "TReplayRecorderReceiver::OnKeyDown : replay recorder cannot cast to type T.";
				return -1;
			}
			return 0;
		}

		virtual int OnKeyUp(T action) {
			if (!isRecording)
				return -1;

			if (replayRecorder != nullptr) {
				if (dynamic_cast<TReplayRecorder<T>*>(replayRecorder) != nullptr) {
					dynamic_cast<TReplayRecorder<T>*>(replayRecorder)->OnKeyUp(action);
					return 0;
				}
				LOG(LogLevel::Error) << "TReplayRecorderReceiver::OnKeyUp : replay recorder cannot cast to type T.";
				return -1;
			}
			return 0;
		}

		virtual int OnButtonDown(T action) {
			if (!isRecording)
				return -1;

			if (replayRecorder != nullptr) {
				if (dynamic_cast<TReplayRecorder<T>*>(replayRecorder) != nullptr) {
					dynamic_cast<TReplayRecorder<T>*>(replayRecorder)->OnButtonDown(action);
					return 0;
				}
				LOG(LogLevel::Error) << "TReplayRecorderReceiver::OnButtonDown : replay recorder cannot cast to type T.";
				return -1;
			}
			return 0;
		}

		virtual int OnButtonUp(T action) {
			if (!isRecording)
				return -1;

			if (replayRecorder != nullptr) {
				if (dynamic_cast<TReplayRecorder<T>*>(replayRecorder) != nullptr) {
					dynamic_cast<TReplayRecorder<T>*>(replayRecorder)->OnButtonUp(action);
					return 0;
				}
				LOG(LogLevel::Error) << "TReplayRecorderReceiver::OnButtonUp : replay recorder cannot cast to type T.";
				return -1;
			}
			return 0;
		}

		virtual int OnKnobTurn(pair<T, int> action) {
			if (!isRecording)
				return -1;

			if (replayRecorder != nullptr) {
				if (dynamic_cast<TReplayRecorder<T>*>(replayRecorder) != nullptr) {
					dynamic_cast<TReplayRecorder<T>*>(replayRecorder)->OnKnobTurn(action);
					return 0;
				}
				LOG(LogLevel::Error) << "TReplayRecorderReceiver::OnKnobTurn : replay recorder cannot cast to type T.";
				return -1;
			}
			return 0;
		}

		virtual int OnSlide(pair<T, int> action) {
			if (!isRecording)
				return -1;

			if (replayRecorder != nullptr) {
				if (dynamic_cast<TReplayRecorder<T>*>(replayRecorder) != nullptr) {
					dynamic_cast<TReplayRecorder<T>*>(replayRecorder)->OnSlide(action);
					return 0;
				}
				LOG(LogLevel::Error) << "TReplayRecorderReceiver::OnSlide : replay recorder cannot cast to type T.";
				return -1;
			}
			return 0;
		}

	};


}}}

#endif