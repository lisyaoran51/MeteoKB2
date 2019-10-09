#include "CustomInputManager.h"


using namespace Framework::Input;




vector<InputHandler*>* CustomInputManager::getInputHandlers()
{
	return &inputHandlers;
}

int CustomInputManager::AddHandler(InputHandler * inputHandler)
{
	// TODO: 以後要寫replay的時候再用
	return 0;
}

int CustomInputManager::RemoveHandler(InputHandler * inputHandler)
{
	// TODO: 以後要寫replay的時候再用
	return 0;
}
