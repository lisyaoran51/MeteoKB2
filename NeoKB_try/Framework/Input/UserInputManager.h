#ifndef USER_INPUT_MANAGER_H
#define USER_INPUT_MANAGER_H

#include "KeyBindings/KeyBindingInputManager.h"

using namespace Framework::Input::KeyBindings;

namespace Framework {
namespace Input {

	class UserInputManager : public KeyBindingInputManager<FrameworkAction> {


	protected:

		virtual vector<InputHandler*>* getInputHandlers();



	};



	enum class FrameworkAction {

	};

}}



#endif