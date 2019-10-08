#include "InputManager.h"


using namespace Framework::Input;



InputManager::InputManager(): RegisterType("InputManager"), Container()
{
	currentState = new InputState();
}

int InputManager::update()
{
	vector<InputState*> pendingStates;
	getPendingState(&pendingStates);

	/* 這邊本來要做create distinct states，這樣可以確保舊的輸入沒被更動，但是現在懶得做 */
	
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
	for (int i = 0; i < getInputHandlers()->size(); i++) {

		vector<InputState*>* inputHandlerPendingState;
		inputHandlerPendingState = getInputHandlers()->at(i)->GetPendingStates();
		pendingStates->reserve(inputHandlerPendingState->size());
		pendingStates->insert(pendingStates->end(), inputHandlerPendingState->begin(), inputHandlerPendingState->end());
		
		delete inputHandlerPendingState;
	}

	return pendingStates;
}

vector<InputState*>* InputManager::createDistinctInputStates(vector<InputState*>* states)
{
	InputState* last = currentState;

	// 這個是用來確保舊的輸入沒被更動，太麻煩懶得寫



	return nullptr;
}

int InputManager::updateInputQueue(InputState * inputState)
{

	triggerQueue.clear();

	// input manager要在加ㄧ個piano input，才會彈琴有聲音
	/*	這一段放到handle key down裡面，不是每個輸入都要給piano處理
	if (staticTriggerQueue.size() > 0) {
		triggerQueue.reserve(staticTriggerQueue.size());
		triggerQueue.insert(triggerQueue.end(), staticTriggerQueue.begin(), staticTriggerQueue.end());
	}*/

	if (focusTriggerable != nullptr)
		triggerQueue.push_back(focusTriggerable);
	else {

		for (int i = 0; i < GetChilds()->size(); i++) {
			Triggerable* temp = GetChilds()->at(i)->Cast<Triggerable>();
			if (temp->GetIsInputable() && temp->GetIsValidForTrigger())
				triggerQueue.push_back(temp);
			iterateGetChild(temp, &triggerQueue);
		}
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
		if (!keyboardState->Contain(lastKeyboardState->GetPresses()->at(i).first)) {
			handleKeyUp(inputState, lastKeyboardState->GetPresses()->at(i).first);
		}
	}

	for (int i = 0; i < keyboardState->GetPresses()->size(); i++) {
		if (!lastKeyboardState->Contain(keyboardState->GetPresses()->at(i).first)) {
			handleKeyDown(inputState, keyboardState->GetPresses()->at(i).first);
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
			handleButtonUp(inputState, lastPanelState->GetButtons()->at(i));
		}
	}

	for (int i = 0; i < panelState->GetButtons()->size(); i++) {
		if (!lastPanelState->ContainButton(panelState->GetButtons()->at(i))) {
			handleButtonDown(inputState, panelState->GetButtons()->at(i));
		}
	}

	/* Knob */
	for (int i = 0; i < panelState->GetKnobs()->size(); i++) {
		if (panelState->GetKnobs()->at(i).second != 0) {
			handleKnobTurn(inputState, panelState->GetKnobs()->at(i).first);
		}
	}

	/* Slider */
	for (int i = 0; i < panelState->GetSliders()->size(); i++) {
		if (panelState->GetSliders()->at(i).second != lastPanelState->GetSliders()->at(i).second) {
			handleSlide(inputState, panelState->GetSliders()->at(i).first);
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

int InputManager::handleKeyDown(InputState * state, InputKey key)
{
	vector<Triggerable*> queue;
	queue.assign(triggerQueue.begin(), triggerQueue.end());
	queue.reserve(triggerQueue.size() + staticTriggerQueue.size());
	queue.insert(queue.end(), staticTriggerQueue.begin(), staticTriggerQueue.end());
	

	return propagateKeyDown(&queue, state, key);
}

int InputManager::propagateKeyDown(vector<Triggerable*>* queue, InputState * state, InputKey key)
{
	for (int i = 0; i < queue->size(); i++) {
		queue->at(i)->TriggerOnKeyDown(state, key);
	}
	return 0;
}

int InputManager::handleKeyUp(InputState * state, InputKey key)
{
	vector<Triggerable*> queue;
	queue.assign(triggerQueue.begin(), triggerQueue.end());
	queue.reserve(triggerQueue.size() + staticTriggerQueue.size());
	queue.insert(queue.end(), staticTriggerQueue.begin(), staticTriggerQueue.end());


	return propagateKeyUp(&queue, state, key);;
}

int InputManager::propagateKeyUp(vector<Triggerable*>* queue, InputState * state, InputKey key)
{
	for (int i = 0; i < queue->size(); i++) {
		queue->at(i)->TriggerOnKeyUp(state, key);
	}
	return 0;
}

int InputManager::handleButtonDown(InputState * state, InputKey button)
{
	vector<Triggerable*> queue;
	queue.assign(triggerQueue.begin(), triggerQueue.end());
	queue.reserve(triggerQueue.size() + staticTriggerQueue.size());
	queue.insert(queue.end(), staticTriggerQueue.begin(), staticTriggerQueue.end());

	return propagateButtonDown(&queue, state, button);
}

int InputManager::propagateButtonDown(vector<Triggerable*>* queue, InputState * state, InputKey button)
{
	for (int i = 0; i < queue->size(); i++) {
		queue->at(i)->TriggerOnButtonDown(state, button);
	}
	return 0;
}

int InputManager::handleButtonUp(InputState * state, InputKey button)
{
	vector<Triggerable*> queue;
	queue.assign(triggerQueue.begin(), triggerQueue.end());
	queue.reserve(triggerQueue.size() + staticTriggerQueue.size());
	queue.insert(queue.end(), staticTriggerQueue.begin(), staticTriggerQueue.end());

	return propagateButtonUp(&queue, state, button);
}

int InputManager::propagateButtonUp(vector<Triggerable*>* queue, InputState * state, InputKey button)
{
	for (int i = 0; i < queue->size(); i++) {
		queue->at(i)->TriggerOnButtonUp(state, button);
	}

	return 0;
}

int InputManager::handleKnobTurn(InputState * state, InputKey knob)
{
	vector<Triggerable*> queue;
	queue.assign(triggerQueue.begin(), triggerQueue.end());
	queue.reserve(triggerQueue.size() + staticTriggerQueue.size());
	queue.insert(queue.end(), staticTriggerQueue.begin(), staticTriggerQueue.end());

	return propagateKnobTurn(&queue, state, knob);;
}

int InputManager::propagateKnobTurn(vector<Triggerable*>* queue, InputState * state, InputKey knob)
{
	for (int i = 0; i < queue->size(); i++) {
		queue->at(i)->TriggerOnKnobTurn(state, knob);
	}

	return 0;
}

int InputManager::handleSlide(InputState * state, InputKey slider)
{
	vector<Triggerable*> queue;
	queue.assign(triggerQueue.begin(), triggerQueue.end());
	queue.reserve(triggerQueue.size() + staticTriggerQueue.size());
	queue.insert(queue.end(), staticTriggerQueue.begin(), staticTriggerQueue.end());

	return propagateSlide(&queue, state, slider);
}

int InputManager::propagateSlide(vector<Triggerable*>* queue, InputState * state, InputKey slider)
{
	for (int i = 0; i < queue->size(); i++) {
		queue->at(i)->TriggerOnSlide(state, slider);
	}
	return 0;
}

int InputManager::load()
{
	return 0;
}

int InputManager::iterateGetChild(Triggerable * temp, vector<Triggerable*>* tQueue)
{
	int size = tQueue->size();

	for (int i = 0; i < temp->GetChilds()->size(); i++) {

		Triggerable* tempChild = GetChilds()->at(i)->Cast<Triggerable>();

		if (tempChild->GetIsInputable())
			iterateGetChild(tempChild, tQueue);
	}

	return size == tQueue->size() ? 0 : -1;
}
