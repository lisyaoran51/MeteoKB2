#ifndef KEY_BINDING_INPUT_MANAGER_H
#define KEY_BINDING_INPUT_MANAGER_H

#include "../PassThroughInputManager.h"
#include "KeyBinding.h"
#include "InputKey.h"
#include <utility>


using namespace Framework::Input;
using namespace std;


namespace Framework {
namespace Input {
namespace KeyBindings {

	template<typename T>
	class KeyBindingInputManager : public PassThroughInputManager {

	public:

		virtual vector<KeyBinding*>* GetDefaultkeyBindings() = 0;

	protected:

		vector<KeyBinding*> keyBindings;
		
		/// <summary>
		/// �bload�������ɭԡA�ɰO�n���檺�u�@
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
		/// �|��queue�̭���keybindingReceivable��X�ӡA�M��I�sonKeyDown(pair<T, int> key)
		/// �n�B�O�u�ݭn��key�A���ά�input state�A���²��ާ@
		/// </summary>
		virtual int propagateKeyDown(vector<Triggerable*>* queue, pair<T, int> key) {

		}

		virtual int propagateKeyUp(vector<Triggerable*>* queue, InputState* state, InputKey key);

		virtual int propagateKeyUp(vector<Triggerable*>* queue, pair<T, int> key);

		virtual int propagateButtonDown(vector<Triggerable*>* queue, InputState* state, InputKey button);

		virtual int propagateButtonDown(vector<Triggerable*>* queue, T key);

		virtual int propagateButtonUp(vector<Triggerable*>* queue, InputState* state, InputKey button);

		virtual int propagateButtonUp(vector<Triggerable*>* queue, T key);

		virtual int propagateKnobTurn(vector<Triggerable*>* queue, InputState* state, InputKey knob);

		virtual int propagateKnobTurn(vector<Triggerable*>* queue, pair<T, int> key);

		virtual int propagateSlide(vector<Triggerable*>* queue, InputState* state, InputKey slider);

		virtual int propagateSlide(vector<Triggerable*>* queue, pair<T, int> key);

	private:

		int handleNewKeyDown(InputState* state, pair<InputKey, int> newKey) {

		}

		int handleNewKeyUp(InputState* state, pair<InputKey, int> newKey);

		int handleNewButtonDown(InputState* state, InputKey newKey);

		int handleNewButtonUp(InputState* state, InputKey newKey);

		int handleNewKnobTurn(InputState* state, pair<InputKey, int> newKey);

		int handleNewSlide(InputState* state, pair<InputKey, int> newKey);


	};


}

}}





#endif