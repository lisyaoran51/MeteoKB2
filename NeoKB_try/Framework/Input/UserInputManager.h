#ifndef USER_INPUT_MANAGER_H
#define USER_INPUT_MANAGER_H

#include "Messages/MessageInputManager.h"
#include "KeyBindings/KeyBindingInputManager.h"


using namespace Framework::Input::Messages;
using namespace Framework::Input::KeyBindings;

namespace Framework {
namespace Input {

	enum class FrameworkAction;

	template<typename TMessage>
	class UserInputManager : public MessageInputManager<FrameworkAction, TMessage> {

	public:

		UserInputManager() : MessageInputManager<FrameworkAction, TMessage>(), RegisterType("UserInputManager")
		{
			this->useParentState = false;
		}

		virtual vector<KeyBinding*>* GetDefaultkeyBindings() {
			return new vector<KeyBinding*>();
		}

	protected:

		virtual vector<InputHandler*>* getInputHandlers() {
			return this->host->GetAvailableInputHandlers();
		}



	};



	enum class FrameworkAction {

	};

}}



#endif