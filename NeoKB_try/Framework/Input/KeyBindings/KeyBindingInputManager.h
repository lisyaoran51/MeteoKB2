#ifndef KEY_BINDING_INPUT_MANAGER_H
#define KEY_BINDING_INPUT_MANAGER_H

#include "../InputManager.h"
#include "KeyBinding.h"
#include "InputKey.h"
#include <utility>


using namespace Framework::Input;
using namespace std;


namespace Framework {
namespace Input {
namespace KeyBindings {

	template<typename T>
	class KeyBindingInputManager : public InputManager {

	public:




	protected:

		vector<KeyBinding*> keyBindings;

		virtual int propagateKeyDown(vector<Triggerable*>* queue, InputState* state, Key key);

		/// <summary>
		/// 會把queue裡面的keybindingReceivable抓出來，然後呼叫onKeyDown(pair<T, int> key)
		/// 好處是只需要抓key，不用看input state，比較簡單操作
		/// </summary>
		virtual int propagateKeyDown(vector<Triggerable*>* queue, pair<T, int> key);

		virtual int propagateKeyUp(vector<Triggerable*>* queue, InputState* state, Key key);

		virtual int propagateKeyUp(vector<Triggerable*>* queue, pair<T, int> key);

		virtual int propagateButtonDown(vector<Triggerable*>* queue, InputState* state, PanelButton button);

		virtual int propagateButtonDown(vector<Triggerable*>* queue, T key);

		virtual int propagateButtonUp(vector<Triggerable*>* queue, InputState* state, PanelButton button);

		virtual int propagateButtonUp(vector<Triggerable*>* queue, T key);

		virtual int propagateKnobTurn(vector<Triggerable*>* queue, InputState* state, PanelKnob knob);

		virtual int propagateKnobTurn(vector<Triggerable*>* queue, pair<T, int> key);

		virtual int propagateSlide(vector<Triggerable*>* queue, InputState* state, PanelSlider slider);

		virtual int propagateSlide(vector<Triggerable*>* queue, pair<T, int> key);

	private:

		int handleNewKeyDown(InputState* state, pair<InputKey, int> newKey);

		int handleNewKeyUp(InputState* state, pair<InputKey, int> newKey);

		int handleNewButtonDown(InputState* state, InputKey newKey);

		int handleNewButtonUp(InputState* state, InputKey newKey);

		int handleNewKnobTurn(InputState* state, pair<InputKey, int> newKey);

		int handleNewSlide(InputState* state, pair<InputKey, int> newKey);


	};


}}}





#endif