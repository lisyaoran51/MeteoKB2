#include "CustomInputManager.h"


using namespace Framework::Input;




CustomInputManager::CustomInputManager() : RegisterType("CustomInputManager")
{
}

vector<InputHandler*>* CustomInputManager::getInputHandlers()
{
	return &inputHandlers;
}

int CustomInputManager::AddHandler(InputHandler * inputHandler)
{
	// TODO: �H��n�greplay���ɭԦA��
	return 0;
}

int CustomInputManager::RemoveHandler(InputHandler * inputHandler)
{
	// TODO: �H��n�greplay���ɭԦA��
	return 0;
}
