#include "InputManager.h"


using namespace Framework::Input;



InputManager::InputManager()
{
	currentState = new InputState();
}

int InputManager::update()
{
	vector<InputState*> pendingStates;
	getPendingState(&pendingStates);

	for (int i = 0; i < pendingStates.size(); i++) {
		handleNewState(pendingStates[i]);
	}

	return 0;
}

int InputManager::handleNewState(InputState * state)
{
	bool hasNewKeyboardState = state->GetKeyboardState() != nullptr;
	bool hasNewPanelState = state->GetPanelState() != nullptr;
	bool hasNewBluetoothState = state->GetBluetoothState() != nullptr;

	InputState* last = currentState;
	if (last->GetLastState() != nullptr)
		delete last->GetLastState();

	currentState = state;
	currentState->SetLastState(last);

	updateInputQueue(currentState);

	if (hasNewKeyboardState)
		updateKeyboardEvents(currentState);

	if (hasNewPanelState)
		updatePanelEvents(currentState);

	if (hasNewBluetoothState)
		updateBluetoothEvents(currentState);


	return 0;
}

vector<InputState*>* InputManager::getPendingState(vector<InputState*>* pendingStates)
{
	for (int i = 0; i < inputHandlers.size(); i++) {

		vector<InputState*> inputHandlerPendingState;
		inputHandlers[i]->GetPendingStates(&inputHandlerPendingState);
		pendingState->reserve(inputHandlerPendingState.size());
		pendingState->insert(pendingState->end(), inputHandlerPendingState.begin(), inputHandlerPendingState.end());
	}

	return pendingStates;
}

vector<InputState*>* InputManager::createDistinctInputStates(vector<InputState*>* states)
{
	InputState* last = currentState;





	return nullptr;
}

int InputManager::updateInputQueue(InputState * inputState)
{

	triggerQueue.clear();

	// input manager要在加ㄧ個piano input，才會彈琴有聲音
	if (staticTriggerQueue.size() > 0) {
		triggerQueue.reserve(staticTriggerQueue.size());
		triggerQueue.insert(triggerQueue.end(), staticTriggerQueue.begin(), staticTriggerQueue.end());
	}

	if (focusTriggerable != nullptr)
		triggerQueue.push_back(focusTriggerable);
	else {
		Triggerable* deepestChild = this;
		int deepestDepth = 0;

		for (int i = 0; i < GetChilds()->size(); i++) {
			Triggerable* tempChild = iterateGetDeepestChild(deepestChild, &deepestDepth, 1);
			if (tempChild != nullptr)
				deepestChild = tempChild;
		}
		triggerQueue.push_back(deepestChild);
	}

	
	

	return 0;
}

int InputManager::updateKeyboardEvents(InputState * inputState)
{
	KeyboardState* keyboardState= inputState->GetKeyboardState();
	KeyboardState* lastKeyboardState = inputState->GetLastState()->GetKeyboardState();

	// 我們不考慮repeat，鎖以不用寫得很複雜
	// 暫時也不寫組合鍵，以後再寫


	for (int i = 0; i < lastKeyboardState->GetPresses()->size(); i++) {
		if (!keyboardState->Contain(lastKeyboardState->GetPresses()->at(i).key)) {
			HandleKeyUp(inputState, lastKeyboardState->GetPresses()->at(i));
		}
	}

	for (int i = 0; i < keyboardState->GetPresses()->size(); i++) {
		if (!lastKeyboardState->Contain(keyboardState->GetPresses()->at(i).key)) {
			HandleKeyDown(inputState, keyboardState->GetPresses()->at(i));
		}
	}

	return 0;
}

int InputManager::updatePanelEvents(InputState * inputState)
{
	PanelState* panelState = inputState->GetPanelState();
	PanelState* lastPanelState = inputState->GetLastState()->GetPanelState();

	// 我們不考慮repeat，鎖以不用寫得很複雜
	// 暫時也不寫組合鍵，以後再寫

	/* Button */
	for (int i = 0; i < lastPanelState->GetButtons()->size(); i++) {
		if (!panelState->ContainButton(lastPanelState->GetButtons()->at(i))) {
			HandleButtonUp(inputState, lastPanelState->GetButtons()->at(i));
		}
	}

	for (int i = 0; i < panelState->GetButtons()->size(); i++) {
		if (!lastPanelState->ContainButton(panelState->GetButtons()->at(i))) {
			HandleKeyDown(inputState, panelState->GetButtons()->at(i));
		}
	}

	/* Knob */
	for (int i = 0; i < panelState->GetKnobs()->Size(); i++) {
		if (panelState->GetKnobs()->at(i)->GetDelta() != 0) {
			handleKnobTurn(inputState, panelState->GetKnobs()->at(i));
		}
	}

	/* Slider */
	for (int i = 0; i < panelState->GetSliders()->Size(); i++) {
		if (panelState->GetSliders()->at(i)->GetLevel() != lastPanelState->GetSliders()->at(i)->GetLevel()) {
			handleSlide(inputState, panelState->GetSliders()->at(i));
		}
	}

	return 0;
}

int InputManager::updateBluetoothEvents(InputState * inputState)
{
	BluetoothState* bluetoothState = inputState->GetBluetoothState();
	BluetoothState* lastBluetoothState = inputState->GetLastState()->GetBluetoothState();


	// 還沒想到有什麼互動

	return 0;
}

int InputManager::iterateGetDeepestChild(Triggerable * temp, int* deepestDepth, int tempDepth)
{

	Triggerable* tempDeeperChild = nullptr;

	if (tempDepth > *deepestDepth) {
		tempDeeperChild = temp;
		*deepestDepth = tempDepth;
	}

	for (int i = 0; i < temp->GetChilds()->size(); i++) {
		Triggerable* tempChild = iterateGetDeepestChild(temp->GetChilds()->at(i), deepestDepth, tempDepth + 1);
		if (tempChild != nullptr)
			tempDeeperChild = tempChild;
	}

	return tempDeeperChild;
}
