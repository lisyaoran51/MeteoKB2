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

	class ReplayRecorderReceiver;

	class ReplayRecorder : public Container {

		int load();

		int load(TimeController* t);

	public:

		ReplayRecorder();
		~ReplayRecorder();

		Replay* GetReplay();

		virtual string GetReplayRecorderVersion() = 0;

		virtual ReplayRecorderReceiver* CreateReceiver() = 0;
		
		virtual int SetDefaultKeyBindings(RulesetInfo* rInfo);

		int SetGameControllingPitchState(bool value);

		int SetGameControllingSustainPedal(bool value);

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

		bool isGameControllingPitchState = false;

		bool isGameControllingSustainPedal = false;

	};

	template<typename T>
	class TReplayRecorder : public ReplayRecorder, public KeyBindingHandler<T> {

	public:

		TReplayRecorder(): RegisterType("TReplayRecorder"){}

		virtual int OnDirectRecordKeyDown(pair<InputKey, int> key) {

			for (int i = 0; i < keyBindings->size(); i++) {
				if (key.first == keyBindings->at(i)->Key) {
					onKeyDown(pair<T, int>(keyBindings->at(i)->GetAction<T>(), key.second));
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordKeyUp(InputKey key) {
			for (int i = 0; i < keyBindings->size(); i++) {
				if (key == keyBindings->at(i)->Key) {
					onKeyUp(keyBindings->at(i)->GetAction<T>());
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordButtonDown(InputKey button) {

			if (!isGameControllingPitchState) {
				if (button == InputKey::RaiseOctave || button == InputKey::LowerOctave) {
					return -1;
				}
			}

			if (!isGameControllingSustainPedal) {
				if (button == InputKey::SustainPedal) {
					return -1;
				}
			}

			for (int i = 0; i < keyBindings->size(); i++) {
				if (button == keyBindings->at(i)->Key) {
					onButtonDown(keyBindings->at(i)->GetAction<T>());
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordButtonUp(InputKey button) {

			if (!isGameControllingSustainPedal) {
				if (button == InputKey::SustainPedal) {
					return -1;
				}
			}

			for (int i = 0; i < keyBindings->size(); i++) {
				if (button == keyBindings->at(i)->Key) {
					onButtonUp(keyBindings->at(i)->GetAction<T>());
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordKnobTurn(pair<InputKey, int> knob) {
			for (int i = 0; i < keyBindings->size(); i++) {
				if (knob.first == keyBindings->at(i)->Key) {
					onKnobTurn(pair<T, int>(keyBindings->at(i)->GetAction<T>(), knob.second));
					return 0;
				}
			}
			return 0;
		}

		virtual int OnDirectRecordSlide(pair<InputKey, int> slider) {
			for (int i = 0; i < keyBindings->size(); i++) {
				if (slider.first == keyBindings->at(i)->Key) {
					onSlide(pair<T, int>(keyBindings->at(i)->GetAction<T>(), slider.second));
					return 0;
				}
			}
			return 0;
		}

	protected:

		virtual int onKeyDown(pair<T, int> action) = 0;

		virtual int onKeyUp(T action) = 0;

		virtual int onButtonDown(T action) = 0;

		virtual int onButtonUp(T action) = 0;

		virtual int onKnobTurn(pair<T, int> action) = 0;

		virtual int onSlide(pair<T, int> action) = 0;
	};


}}}





#endif