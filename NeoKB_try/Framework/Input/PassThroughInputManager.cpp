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

int PassThroughInputManager::update()
{
	InputManager::update();

	LOG(LogLevel::Finest) << "PassThroughInputManager::getPendingState(): [" << GetTypeName() << "] start update." << (useParentState ? 1 : 0);

	if(!useParentState)
		for (int i = 0; i < pendingStates.size(); i++) {	// 從input handler創建，到這邊delete掉
			delete pendingStates[i];						//
		}

	pendingStates.clear();

	LOG(LogLevel::Finest) << "PassThroughInputManager::getPendingState(): [" << GetTypeName() << "] update finished.";

	return 0;
}

vector<InputState*>* PassThroughInputManager::getPendingState(vector<InputState*>* pStates)
{
	LOG(LogLevel::Finest) << "PassThroughInputManager::getPendingState(): start getting passing states.";
	/* 先把handlers裡面的pending states全都清出來 */
	InputManager::getPendingState(pStates);

	if (!useParentState)
		return pStates;

	pStates->clear();

	for (int i = 0; i < pendingParentStates.size(); i++) {

		LOG(LogLevel::Debug) << "PassThroughInputManager::getPendingState(): " << GetTypeName() << " passing states.";
		// 很有可能都是重複的同一個input state，所以先確定一下
		if (find(pStates->begin(), pStates->end(), pendingParentStates[i]) == pStates->end())
			pStates->push_back(pendingParentStates[i]);
	}

	pendingParentStates.clear();

	return pStates;
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

int PassThroughInputManager::onCommand(InputState * inputState, BluetoothCommand * bluetoothCommand)
{
	return acceptState(inputState);
}
