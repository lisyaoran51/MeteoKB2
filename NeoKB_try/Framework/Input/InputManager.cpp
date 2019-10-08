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

	/* �o�䥻�ӭn��create distinct states�A�o�˥i�H�T�O�ª���J�S�Q��ʡA���O�{�b�i�o�� */
	
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

	// �o�ӬO�ΨӽT�O�ª���J�S�Q��ʡA�ӳ·��i�o�g



	return nullptr;
}

int InputManager::updateInputQueue(InputState * inputState)
{

	triggerQueue.clear();

	// input manager�n�b�[����piano input�A�~�|�u�^���n��
	/*	�o�@�q���handle key down�̭��A���O�C�ӿ�J���n��piano�B�z
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

	// �ڭ̤��Ҽ{repeat�A��H���μg�o�ܽ���
	// �Ȯɤ]���g�զX��A�H��A�g


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

	// �ڭ̤��Ҽ{repeat�A��H���μg�o�ܽ���
	// �Ȯɤ]���g�զX��A�H��A�g

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


	// �٨S�Q�즳���򤬰�

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
