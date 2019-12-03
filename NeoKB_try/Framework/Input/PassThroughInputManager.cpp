#include "PassThroughInputManager.h"

#include <algorithm>

using namespace Framework::Input;


int PassThroughInputManager::acceptState(InputState * inputState)
{
	if (useParentState)
		pendingParentStates.push_back(inputState);
	return RETURN_FAILED;
}

PassThroughInputManager::PassThroughInputManager() : RegisterType("PassThroughInputManager")
{
	isInputable = true;
	isPresent = true;
}

vector<InputState*>* PassThroughInputManager::getPendingState(vector<InputState*>* pendingStates)
{
	/* 先把handlers裡面的pending states全都清出來 */
	InputManager::getPendingState(pendingStates);

	if (!useParentState)
		return pendingStates;

	pendingStates->clear();

	for (int i = 0; i < pendingParentStates.size(); i++) {
		// 很有可能都是重複的同一個input state，所以先確定一下
		if (find(pendingStates->begin(), pendingStates->end(), pendingParentStates[i]) == pendingStates->end()) 
			pendingStates->push_back(pendingParentStates[i]);
	}

	pendingParentStates.clear();

	return pendingStates;
}

int PassThroughInputManager::onKeyDown(InputState * inputState, InputKey key)
{
	return acceptState(inputState);
}

int PassThroughInputManager::onKeyUp(InputState * inputState, InputKey key)
{
	return acceptState(inputState);
}

int PassThroughInputManager::onKnobTurn(InputState * inputState, InputKey knob)
{
	return acceptState(inputState);
}

int PassThroughInputManager::onButtonDown(InputState * inputState, InputKey button)
{
	return acceptState(inputState);
}

int PassThroughInputManager::onButtonUp(InputState * inputState, InputKey button)
{
	return acceptState(inputState);
}

int PassThroughInputManager::onSlide(InputState * inputState, InputKey slider)
{
	return acceptState(inputState);
}
