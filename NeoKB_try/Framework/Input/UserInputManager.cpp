#include "UserInputManager.h"


using namespace Framework::Input;

vector<InputHandler*>* UserInputManager::getInputHandlers()
{
	return host->GetAvailableInputHandlers();
}
