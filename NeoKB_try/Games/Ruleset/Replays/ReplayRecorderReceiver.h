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
	/// �]��replay recorder�bruleset input manager�W���A�ҥH��B�Druleset��input�A�n��o��receiver�ʦbplayfield�U���A�~����k�h��input�A�M��A��input�Ǩ�ReplayRecorder�h
	/// </summary>
	class ReplayRecorderReceiver : public Container {

		int load();

		int load(ReplayRecorder* r, TimeController* t);

	public:


		ReplayRecorderReceiver();


	protected:

		ReplayRecorder* replayRecorder = nullptr;

		/// <summary>
		/// �ΨӧP�_�O���O�������A�p�G�Ofalse�N�������
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