#ifndef REPLAY_RECORDER_H
#define REPLAY_RECORDER_H


#include "../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../../../Framework/Timing/TimeController.h"
#include "Replay.h"
#include "../RulesetInfo.h"
#include "../../../Framework/Input/KeyBindings/KeyBinding.h"


using namespace Framework::Allocation::Hierachal;
using namespace Framework::Input::KeyBindings;
using namespace Framework::Timing;
using namespace Games::Rulesets;
using namespace Framework::Input::KeyBindings;


namespace Games {
namespace Rulesets {
namespace Replays {

	class ReplayRecorder : public Container {

		int load();

		int load(TimeController* t);

	public:

		ReplayRecorder();
		~ReplayRecorder();

		Replay* GetReplay();

		virtual string GetReplayRecorderVersion() = 0;
		
		virtual int SetDefaultKeyBindings(RulesetInfo* rInfo);

		virtual int OnDirectRecordKeyDown(pair<InputKey, int> key) = 0;

		virtual int OnDirectRecordKeyUp(InputKey key) = 0;

		virtual int OnDirectRecordButtonDown(InputKey button) = 0;

		virtual int OnDirectRecordButtonUp(InputKey button) = 0;

		virtual int OnDirectRecordKnobTurn(pair<InputKey, int> knob) = 0;

		virtual int OnDirectRecordSlide(pair<InputKey, int> slider) = 0;

		//virtual int OnDirectRecordMessage(InputState* inputState, BluetoothMessage* message);

	protected:

		Replay* replay = nullptr;

		TimeController* timeController = nullptr;

		vector<KeyBinding*>* keyBindings = nullptr;;

	};

	template<typename T>
	class TReplayRecorder : public ReplayRecorder, public KeyBindingHandler<T> {

	public:

		TReplayRecorder(): RegisterType("TReplayRecorder"){}

		virtual int OnDirectRecordKeyDown(pair<InputKey, int> key) {

			for (int i = 0; i < keyBindings->size(); i++) {
				if (key.first == keyBindings->at(i)->Key) {
					OnKeyDown(pair<T, int>(keyBindings->at(i)->GetAction<T>(), key.second));
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordKeyUp(InputKey key) {
			for (int i = 0; i < keyBindings->size(); i++) {
				if (key == keyBindings->at(i)->Key) {
					OnKeyUp(keyBindings->at(i)->GetAction<T>());
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordButtonDown(InputKey button) {
			for (int i = 0; i < keyBindings->size(); i++) {
				if (button == keyBindings->at(i)->Key) {
					OnButtonDown(keyBindings->at(i)->GetAction<T>());
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordButtonUp(InputKey button) {
			for (int i = 0; i < keyBindings->size(); i++) {
				if (button == keyBindings->at(i)->Key) {
					OnButtonUp(keyBindings->at(i)->GetAction<T>());
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordKnobTurn(pair<InputKey, int> knob) {
			for (int i = 0; i < keyBindings->size(); i++) {
				if (knob.first == keyBindings->at(i)->Key) {
					OnKnobTurn(pair<T, int>(keyBindings->at(i)->GetAction<T>(), knob.second));
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordSlide(pair<InputKey, int> slider) {
			for (int i = 0; i < keyBindings->size(); i++) {
				if (slider.first == keyBindings->at(i)->Key) {
					OnSlide(pair<T, int>(keyBindings->at(i)->GetAction<T>(), slider.second));
					return 0;
				}
			}
			return 0;
		}


		virtual int OnKeyDown(pair<T, int> action) = 0;

		virtual int OnKeyUp(T action) = 0;

		virtual int OnButtonDown(T action) = 0;

		virtual int OnButtonUp(T action) = 0;

		virtual int OnKnobTurn(pair<T, int> action) = 0;

		virtual int OnSlide(pair<T, int> action) = 0;
	};


}}}





#endif