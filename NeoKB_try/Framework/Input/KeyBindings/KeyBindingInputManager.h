#ifndef KEY_BINDING_INPUT_MANAGER_H
#define KEY_BINDING_INPUT_MANAGER_H

#include "../InputManager.h"


using namespace Framework::Input;

namespace Framework {
namespace Input {
namespace KeyBindings {

	template<typename T>
	class KeyBindingInputManager : public InputManager {

	public:


	protected:

		vector<KeyBinding> keyBindings;

	};


}}}





#endif