#include "UserInputManager.h"


using namespace Framework::Input;

UserInputManager::UserInputManager() : KeyBindingInputManager(), RegisterType("UserInputManager")
{
	useParentState = false;
}

vector<KeyBinding*>* UserInputManager::GetDefaultkeyBindings()
{
	return new vector<KeyBinding*>();
}

vector<InputHandler*>* UserInputManager::getInputHandlers()
{
	return host->GetAvailableInputHandlers();
}
