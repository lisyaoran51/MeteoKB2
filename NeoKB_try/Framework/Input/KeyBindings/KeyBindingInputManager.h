#ifndef KEY_BINDING_INPUT_MANAGER_H
#define KEY_BINDING_INPUT_MANAGER_H

#include "../PassThroughInputManager.h"
#include "KeyBinding.h"
#include "InputKey.h"
#include <utility>
#include "KeyBindingHandler.h"
#include <algorithm>


using namespace Framework::Input;
using namespace std;


namespace Framework {
namespace Input {
namespace KeyBindings {

	template<typename T>
	class KeyBindingInputManager : public PassThroughInputManager {


	public:

		KeyBindingInputManager() : PassThroughInputManager(), RegisterType("KeyBindingInputManager"){

		}

		virtual vector<KeyBinding*>* GetDefaultkeyBindings() = 0;

	protected:

		vector<KeyBinding*> keyBindings;
		
		/// <summary>
		/// 在load結束的時候，時記要執行的工作
		/// </summary>
		virtual int LoadOnCompleted() {

			reloadMappings();
			
			return 0;
		}

		virtual int reloadMappings() {

			vector<KeyBinding*>* defaultKeyBindings = GetDefaultkeyBindings();
			keyBindings.reserve(defaultKeyBindings->size());
			keyBindings.insert(keyBindings.end(), defaultKeyBindings->begin(), defaultKeyBindings->end());

			delete defaultKeyBindings;

			return 0;

		}

		virtual int propagateKeyDown(vector<Triggerable*>* queue, InputState* state, InputKey key) {
			pair<InputKey, int> action = make_pair<InputKey, int>(InputKey::None, 0);
			for (int i = 0; i < state->GetKeyboardState()->GetPresses()->size(); i++) {
				if (state->GetKeyboardState()->GetPresses()->at(i).first == key)
					action = state->GetKeyboardState()->GetPresses()->at(i);
			}

			return InputManager::propagateKeyDown(queue, state, key) + handleNewKeyDown(queue, action);
		}

		/// <summary>
		/// 會把queue裡面的keybindingReceivable抓出來，然後呼叫onKeyDown(pair<T, int> key)
		/// 好處是只需要抓key，不用看input state，比較簡單操作
		/// </summary>
		virtual int propagateKeyDown(vector<Triggerable*>* queue, pair<T, int> key) {
			
			for (int i = 0; i < queue->size(); i++) {
				KeyBindingHandler<T>* keyBindingHandler = dynamic_cast<KeyBindingHandler<T>*>(queue->at(i));
				if (keyBindingHandler != nullptr) {
					keyBindingHandler->OnKeyDown(key);
				}
			}

			return 0;
		}

		virtual int propagateKeyUp(vector<Triggerable*>* queue, InputState* state, InputKey key) {
			return InputManager::propagateKeyUp(queue, state, key) + handleNewKeyUp(queue, key);
		}

		virtual int propagateKeyUp(vector<Triggerable*>* queue, T key) {

			for (int i = 0; i < queue->size(); i++) {
				KeyBindingHandler<T>* keyBindingHandler = dynamic_cast<KeyBindingHandler<T>*>(queue->at(i));
				if (keyBindingHandler != nullptr) {
					keyBindingHandler->OnKeyUp(key);
				}
			}

			return 0;
		}

		virtual int propagateButtonDown(vector<Triggerable*>* queue, InputState* state, InputKey button) {
			return InputManager::propagateButtonDown(queue, state, button) + handleNewButtonDown(queue, button);
		}

		virtual int propagateButtonDown(vector<Triggerable*>* queue, T button) {

			for (int i = 0; i < queue->size(); i++) {
				KeyBindingHandler<T>* keyBindingHandler = dynamic_cast<KeyBindingHandler<T>*>(queue->at(i));
				if (keyBindingHandler != nullptr) {
					keyBindingHandler->OnButtonDown(button);
				}
			}

			return 0;
		}

		virtual int propagateButtonUp(vector<Triggerable*>* queue, InputState* state, InputKey button) {
			return InputManager::propagateButtonDown(queue, state, button) + handleNewButtonUp(queue, button);
		}

		virtual int propagateButtonUp(vector<Triggerable*>* queue, T button) {

			for (int i = 0; i < queue->size(); i++) {
				KeyBindingHandler<T>* keyBindingHandler = dynamic_cast<KeyBindingHandler<T>*>(queue->at(i));
				if (keyBindingHandler != nullptr) {
					keyBindingHandler->OnButtonUp(button);
				}
			}

			return 0;
		}

		virtual int propagateKnobTurn(vector<Triggerable*>* queue, InputState* state, InputKey knob) {

			pair<InputKey, int> action = make_pair<InputKey, int>(InputKey::None, 0);
			for (int i = 0; i < state->GetPanelState()->GetKnobs()->size(); i++) {
				if (state->GetPanelState()->GetKnobs()->at(i).first == knob)
					action = state->GetPanelState()->GetKnobs()->at(i);
			}

			return InputManager::propagateKnobTurn(queue, state, knob) + handleNewKnobTurn(queue, action);
		}

		virtual int propagateKnobTurn(vector<Triggerable*>* queue, pair<T, int> knob) {

			for (int i = 0; i < queue->size(); i++) {
				KeyBindingHandler<T>* keyBindingHandler = dynamic_cast<KeyBindingHandler<T>*>(queue->at(i));
				if (keyBindingHandler != nullptr) {
					keyBindingHandler->OnKnobTurn(knob);
				}
			}

			return 0;

		}

		virtual int propagateSlide(vector<Triggerable*>* queue, InputState* state, InputKey slider) {

			pair<InputKey, int> action = make_pair<InputKey, int>(InputKey::None, 0);
			for (int i = 0; i < state->GetPanelState()->GetSliders()->size(); i++) {
				if (state->GetPanelState()->GetSliders()->at(i).first == slider)
					action = state->GetPanelState()->GetSliders()->at(i);
			}

			return InputManager::propagateSlide(queue, state, slider) + handleNewSlide(queue, action);

		}

		virtual int propagateSlide(vector<Triggerable*>* queue, pair<T, int> Slider) {

			for (int i = 0; i < queue->size(); i++) {
				KeyBindingHandler<T>* keyBindingHandler = dynamic_cast<KeyBindingHandler<T>*>(queue->at(i));
				if (keyBindingHandler != nullptr) {
					keyBindingHandler->OnSlide(Slider);
				}
			}

			return 0;

		}

	private:



		int handleNewKeyDown(vector<Triggerable*>* queue, pair<InputKey, int> newKey) {

			vector<KeyBinding*>::iterator it = find_if(keyBindings.begin(), keyBindings.end(), [=](const KeyBinding& k)->bool {
				if (k.Key == newKey.first) return true;
				else return false;
			});


			if (it != keyBindings.end()) {
				propagateKeyDown(queue, pair<T, int>((*it)->GetAction<T>(), newKey.second));
				return 0;
			}
			else {
				return -1;
			}

			return 0;
		}

		int handleNewKeyUp(vector<Triggerable*>* queue, InputKey newKey) {

			vector<KeyBinding*>::iterator it = find_if(keyBindings.begin(), keyBindings.end(), [=](const KeyBinding& k)->bool {
				if (k.Key == newKey) return true;
				else return false;
			});

			if (it != keyBindings.end()) {
				propagateKeyUp(queue, (T)(*it)->GetAction<T>());
				return 0;
			}
			else {
				return -1;
			}

		}

		int handleNewButtonDown(vector<Triggerable*>* queue, InputKey newButton) {

			vector<KeyBinding*>::iterator it = find_if(keyBindings.begin(), keyBindings.end(), [=](const KeyBinding& k)->bool {
				if (k.Key == newButton) return true;
				else return false;
			});

			if (it != keyBindings.end()) {
				propagateButtonDown(queue, (T)(*it)->GetAction<T>());
				return 0;
			}
			else {
				return -1;
			}

		}

		int handleNewButtonUp(vector<Triggerable*>* queue, InputKey newButton) {

			vector<KeyBinding*>::iterator it = find_if(keyBindings.begin(), keyBindings.end(), [=](const KeyBinding& k)->bool {
				if (k.Key == newButton) return true;
				else return false;
			});

			if (it != keyBindings.end()) {
				propagateButtonUp(queue, (T)(*it)->GetAction<T>());
				return 0;
			}
			else {
				return -1;
			}

		}

		int handleNewKnobTurn(vector<Triggerable*>* queue, pair<InputKey, int> newKnob) {

			vector<KeyBinding*>::iterator it = find_if(keyBindings.begin(), keyBindings.end(), [=](const KeyBinding& k)->bool {
				if (k.Key == newKnob.first) return true;
				else return false;
			});

			if (it != keyBindings.end()) {
				propagateKnobTurn(queue, pair<T, int>((T)(*it)->GetAction<T>(), newKnob.second));
				return 0;
			}
			else {
				return -1;
			}

		}

		int handleNewSlide(vector<Triggerable*>* queue, pair<InputKey, int> newSlider) {

			vector<KeyBinding*>::iterator it = find_if(keyBindings.begin(), keyBindings.end(), [=](const KeyBinding& k)->bool {
				if (k.Key == newSlider.first) return true;
				else return false;
			});

			if (it != keyBindings.end()) {
				propagateSlide(queue, pair<T, int>((T)(*it)->GetAction<T>(), newSlider.second));
				return 0;
			}
			else {
				return -1;
			}

			return 0;

		}



	};


}

}}





#endif