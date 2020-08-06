#ifndef USER_INPUT_MANAGER_H
#define USER_INPUT_MANAGER_H

#include "Commands/CommandInputManager.h"
#include "KeyBindings/KeyBindingInputManager.h"


using namespace Framework::Input::Commands;
using namespace Framework::Input::KeyBindings;

namespace Framework {
namespace Input {

	enum class FrameworkAction;

	//template<typename TCommand>
	//class UserInputManager : public CommandInputManager<FrameworkAction, TCommand> {
	class UserInputManager :public KeyBindingInputManager<FrameworkAction>{

	public:

		//UserInputManager() : CommandInputManager<FrameworkAction, TCommand>(), RegisterType("UserInputManager")
		UserInputManager() :KeyBindingInputManager<FrameworkAction>(), RegisterType("UserInputManager")
		{
			useParentState = false;
		}

		virtual vector<KeyBinding*>* GetDefaultkeyBindings() {
			return new vector<KeyBinding*>();
		}

	protected:

		virtual vector<InputHandler*>* getInputHandlers() {
			return host->GetAvailableInputHandlers();
		}



	};



	enum class FrameworkAction {

	};

}}



#endif