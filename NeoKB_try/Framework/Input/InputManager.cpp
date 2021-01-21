#include "InputManager.h"


using namespace Framework::Input;



InputManager::InputManager(): RegisterType("InputManager")
{
	registerLoad(bind(static_cast<int(InputManager::*)(void)>(&InputManager::load), this));
	currentState = new InputState();
	
}

int InputManager::ChangeFocus(Triggerable * fTriggerable)
{
	// TODO: 之後再寫
	return 0;
}


int InputManager::update()
{
	LOG(LogLevel::Depricated) << "InputManager::update(): start update.";
	getPendingState(&pendingStates);
	LOG(LogLevel::Depricated) << "InputManager::update(): get pending states.";
	if(pendingStates.size() > 0)
		LOG(LogLevel::Finest) << "InputManager::update(): get [" << pendingStates[0] << "] states by " << GetTypeName() << ".";

	/* 這邊本來要做create distinct states，這樣可以確保舊的輸入沒被更動，經過這個以後panel.keyboard.bt都部會是null，但裡面會是沒有東西的 */
	vector<InputState*>* distinctInputStates = createDistinctInputStates(&pendingStates);
	
	LOG(LogLevel::Depricated) << "InputManager::update():create distinct input state [" << distinctInputStates->at(0) << "] by " << GetTypeName() << ".";

	//LOG(LogLevel::Debug) << "InputManager::update() : distinct input states size = [" << distinctInputStates->size() << "].";
	for (int i = 0; i < distinctInputStates->size(); i++) {
		handleNewState(distinctInputStates->at(i));
	}

	LOG(LogLevel::Depricated) << "InputManager::update(): after handling states.";

	delete distinctInputStates;
	LOG(LogLevel::Depricated) << "InputManager::update(): delete states.";

	return 0;
}

int InputManager::handleNewState(InputState * state)
{

	bool hasNewKeyboardState = !state->GetKeyboardState()->CheckIsEmpty();
	bool hasNewPanelState = !state->GetPanelState()->CheckIsEmpty();
	bool hasNewBluetoothState = !state->GetBluetoothState()->CheckIsEmpty();

	LOG(LogLevel::Depricated) << "InputManager::handleNewState(): set last state is [" << currentState << "] by [" << GetTypeName() << "].";

	InputState* last = currentState;

	currentState = state;

	LOG(LogLevel::Depricated) << "InputManager::handleNewState(): delete last state is [" << last->GetLastState() << "], state->bt is [" << last->GetBluetoothState() << "], by [" << GetTypeName() << "].";
	currentState->SetLastState(last);

	LOG(LogLevel::Depricated) << "InputManager::handleNewState(): setting last state.";

	//unique_lock<mutex> uLock(TreeMutex1);
	updateInputQueue(currentState);

	if (hasNewKeyboardState)
		updateKeyboardEvents(currentState);

	if (hasNewPanelState)
		updatePanelEvents(currentState);

	if (hasNewBluetoothState)
		updateBluetoothEvents(currentState);

	return 0;
}

vector<InputState*>* InputManager::getPendingState(vector<InputState*>* pStates)
{
	for (int i = 0; i < getInputHandlers()->size(); i++) {

		vector<InputState*>* inputHandlerPendingState;
		inputHandlerPendingState = getInputHandlers()->at(i)->GetPendingStates();



		if (inputHandlerPendingState->size() > 0) {
			pStates->reserve(inputHandlerPendingState->size());
			pStates->insert(pStates->end(), inputHandlerPendingState->begin(), inputHandlerPendingState->end());
		
			if (inputHandlerPendingState->at(0)->GetPanelState())
				LOG(LogLevel::Depricated) << "InputManager::getPendingState() : get panel input.";

			if (inputHandlerPendingState->at(0)->GetBluetoothState() && inputHandlerPendingState->at(0)->GetBluetoothState()->GetCommands()->size() > 0) {
				LOG(LogLevel::Depricated) << "InputManager::getPendingState() : receive bt input.";
				LOG(LogLevel::Depricated) << "InputManager::getPendingState() : get bt input [" << inputHandlerPendingState->at(0)->GetBluetoothState()->GetCommands()->at(0)->GetContext().dump() << "].";
			}
		}

		delete inputHandlerPendingState;
	}
	return pStates;
}

vector<InputState*>* InputManager::createDistinctInputStates(vector<InputState*>* states)
{
	// 這個是用來確保舊的輸入沒被更動，並且把所有的input state擺進一個distinct state裡
	vector<InputState*>* returnValue = new vector<InputState*>();
	InputState* distinctState = new InputState();
	returnValue->push_back(distinctState);

	for (int i = 0; i < states->size(); i++) {


		InputState* state = states->at(i);

		bool hasNewKeyboardState = state->GetKeyboardState() != nullptr;
		bool hasNewPanelState = state->GetPanelState() != nullptr;
		bool hasNewBluetoothState = state->GetBluetoothState() != nullptr;

		/* Keyboard State */
		if (hasNewKeyboardState) {
			if (distinctState->GetKeyboardState() == nullptr)
				distinctState->SetKeyboardState(new KeyboardState());
			KeyboardState* newKeyboardState = distinctState->GetKeyboardState();

			/* press */
			for (int i = 0; i < state->GetKeyboardState()->GetPresses()->size(); i++) {
				pair<InputKey, int> press = state->GetKeyboardState()->GetPresses()->at(i);
				//檢查有沒有重複
				for (int j = 0; j < newKeyboardState->GetPresses()->size(); j++) {
					if (newKeyboardState->GetPresses()->at(j).first == press.first) {
						// 把重複的刪掉，只留最新的
						newKeyboardState->GetPresses()->erase(newKeyboardState->GetPresses()->begin() + j);
						j--;
					}
				}
				newKeyboardState->GetPresses()->push_back(press);
			}

			/* up */
			for (int i = 0; i < state->GetKeyboardState()->GetUps()->size(); i++) {
				InputKey up = state->GetKeyboardState()->GetUps()->at(i);
				//檢查有沒有重複
				for (int j = 0; j < newKeyboardState->GetUps()->size(); j++) {
					if (newKeyboardState->GetUps()->at(j) == up) {
						// 把重複的刪掉，只留最新的
						newKeyboardState->GetUps()->erase(newKeyboardState->GetUps()->begin() + j);
						j--;
					}
				}
				newKeyboardState->GetUps()->push_back(up);
			}
		}


		/* Panel State */
		if (hasNewPanelState) {
			if (distinctState->GetPanelState() == nullptr)
				distinctState->SetPanelState(new PanelState());
			PanelState* newPanelState = distinctState->GetPanelState();

			/* button */
			for (int i = 0; i < state->GetPanelState()->GetButtons()->size(); i++) {
				InputKey button = state->GetPanelState()->GetButtons()->at(i);
				//檢查有沒有重複
				bool duplicated = false;
				for (int j = 0; j < newPanelState->GetButtons()->size(); j++) {
					if (newPanelState->GetButtons()->at(j) == button) {
						// 有重複就直接跳過
						duplicated = true;
						break;
					}
				}
				if(!duplicated)
					newPanelState->GetButtons()->push_back(button);
			}

			/* knob */
			for (int i = 0; i < state->GetPanelState()->GetKnobs()->size(); i++) {
				pair<InputKey, int> knob = state->GetPanelState()->GetKnobs()->at(i);
				//檢查有沒有重複
				for (int j = 0; j < newPanelState->GetKnobs()->size(); j++) {
					if (newPanelState->GetKnobs()->at(j).first == knob.first) {
						// 把重複的刪掉，只留最新的
						newPanelState->GetKnobs()->erase(newPanelState->GetKnobs()->begin() + j);
						j--;
					}
				}
				newPanelState->GetKnobs()->push_back(knob);
			}

			/* slider */
			for (int i = 0; i < state->GetPanelState()->GetSliders()->size(); i++) {
				pair<InputKey, int> slider = state->GetPanelState()->GetSliders()->at(i);
				//檢查有沒有重複
				for (int j = 0; j < newPanelState->GetSliders()->size(); j++) {
					if (newPanelState->GetSliders()->at(j).first == slider.first) {
						// 把重複的刪掉，只留最新的
						newPanelState->GetSliders()->erase(newPanelState->GetSliders()->begin() + j);
						j--;
					}
				}
				newPanelState->GetSliders()->push_back(slider);
			}
		}


		/* Bluetooth State */
		if (hasNewBluetoothState) {
			if (distinctState->GetBluetoothState() == nullptr)
				distinctState->SetBluetoothState(new BluetoothState());
			BluetoothState* newBluetoothState = distinctState->GetBluetoothState();

			

			/* bt */
			for (int i = 0; i < state->GetBluetoothState()->GetMessages()->size(); i++) {
				LOG(LogLevel::Finest) << "InputManager::createDistinctInputStates(): cloning bt command [" << state->GetBluetoothState()->GetMessages()->at(i) << "] into distinct state." << state->GetBluetoothState()->GetCommands()->at(i)->GetContext().dump();

				BluetoothMessage* bluetoothMessage = state->GetBluetoothState()->GetMessages()->at(i);
				newBluetoothState->GetMessages()->push_back(bluetoothMessage->Clone());

				LOG(LogLevel::Finest) << "InputManager::createDistinctInputStates(): cloning over.";

			}
		}
	}

	/* 如果沒有State的話，要擺一個空的，這樣才能比較上一次和這一次輸入差多少 */
	if (distinctState->GetKeyboardState() == nullptr)
		distinctState->SetKeyboardState(new KeyboardState());
	if (distinctState->GetPanelState() == nullptr)
		distinctState->SetPanelState(new PanelState());
	if (distinctState->GetBluetoothState() == nullptr)
		distinctState->SetBluetoothState(new BluetoothState());

	return returnValue;
}

int InputManager::TransformState(InputState * inputState)
{
	// 還不知道這個要幹嘛 先不要寫
	return 0;
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
		vector<ChildAddable*> childs;

		unique_lock<mutex> uLock(ChildMutex);
		childs.assign(GetChilds()->begin(), GetChilds()->end());
		uLock.unlock();

		for (int i = 0; i < childs.size(); i++) {
			Triggerable* temp = childs[i]->Cast<Triggerable>();
			if (temp->GetIsInputReceivable())
				triggerQueue.push_back(temp);
			iterateUpdateInputQueue(temp, &triggerQueue);
		}
	}

	return 0;
}

int InputManager::updateKeyboardEvents(InputState * inputState)
{
	LOG(LogLevel::Depricated) << "InputManager::updateKeyboardEvents() : updateing fake input.";

	KeyboardState* keyboardState= inputState->GetKeyboardState();
	KeyboardState* lastKeyboardState = inputState->GetLastState() ? inputState->GetLastState()->GetKeyboardState() : nullptr;

	// 我們不考慮repeat，鎖以不用寫得很複雜
	// 暫時也不寫組合鍵，以後再寫

	for (int i = 0; i < keyboardState->GetPresses()->size(); i++) {
		handleKeyDown(inputState, keyboardState->GetPresses()->at(i).first);
	}

	for (int i = 0; i < keyboardState->GetUps()->size(); i++) {
		handleKeyUp(inputState, keyboardState->GetUps()->at(i));
	}

	if (lastKeyboardState) {
	}
	else {
	}

	return 0;
}

int InputManager::updatePanelEvents(InputState * inputState)
{
	LOG(LogLevel::Depricated) << "InputManager::updatePanelEvents() : updateing fake input.";

	PanelState* panelState = inputState->GetPanelState();
	PanelState* lastPanelState = inputState->GetLastState() ? inputState->GetLastState()->GetPanelState() : nullptr;

	// 我們不考慮repeat，鎖以不用寫得很複雜
	// 暫時也不寫組合鍵，以後再寫

	if (lastPanelState) {
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
	}
	else {
		for (int i = 0; i < panelState->GetButtons()->size(); i++) {
			handleButtonDown(inputState, lastPanelState->GetButtons()->at(i));
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
	BluetoothState* lastBluetoothState = inputState->GetLastState() ? inputState->GetLastState()->GetBluetoothState() : nullptr;

	LOG(LogLevel::Finest) << "InputManager::updateBluetoothEvents() : trigger queue- by " << GetTypeName();
	LOG(LogLevel::Depricated) << [](vector<Triggerable*>& triggerables) {

		for (int i = 0; i < triggerables.size(); i++)
			LOG(LogLevel::Depricated) << "|---------[" << triggerables[i]->GetTypeName() << "]";

		return 0;
	}(triggerQueue);

	for (int i = 0; i < bluetoothState->GetMessages()->size(); i++) {
		handleBluetoothMessage(inputState, bluetoothState->GetMessages()->at(i));
	}

	// 還沒想到有什麼互動

	return 0;
}

int InputManager::handleKeyDown(InputState * state, InputKey key)
{
	LOG(LogLevel::Finest) << "InputManager::handleKeyDown() : trigger queue- by " << GetTypeName();
	LOG(LogLevel::Depricated) << [](vector<Triggerable*>& triggerables) {

		for (int i = 0; i < triggerables.size(); i++)
			LOG(LogLevel::Depricated) << "|---------[" << triggerables[i]->GetTypeName() << "]";

		return 0;
	}(triggerQueue);

	return propagateKeyDown(&triggerQueue, state, key);
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
	return propagateKeyUp(&triggerQueue, state, key);;
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
	LOG(LogLevel::Depricated) << "InputManager::handleButtonDown() : trigger queue- by " << GetTypeName();
	LOG(LogLevel::Depricated) << [](vector<Triggerable*>& triggerables) {

		for(int i = 0; i < triggerables.size(); i++)
			LOG(LogLevel::Debug) << "|---------[" << triggerables[i]->GetTypeName() << "]";

		return 0;
	}(triggerQueue);

	return propagateButtonDown(&triggerQueue, state, button);
}

int InputManager::propagateButtonDown(vector<Triggerable*>* queue, InputState * state, InputKey button)
{
	LOG(LogLevel::Depricated) << "InputManager::propagateButtonDown() : original propogate process.";

	for (int i = 0; i < queue->size(); i++) {
		LOG(LogLevel::Depricated) << "InputManager::propagateButtonDown() : original propogate process trigger [" << i << "] in [" << queue->size() << "] triggerables.";
		queue->at(i)->TriggerOnButtonDown(state, button);
	}

	return 0;
}

int InputManager::handleButtonUp(InputState * state, InputKey button)
{
	return propagateButtonUp(&triggerQueue, state, button);
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
	return propagateKnobTurn(&triggerQueue, state, knob);;
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
	return propagateSlide(&triggerQueue, state, slider);
}

int InputManager::propagateSlide(vector<Triggerable*>* queue, InputState * state, InputKey slider)
{
	for (int i = 0; i < queue->size(); i++) {
		queue->at(i)->TriggerOnSlide(state, slider);
	}
	return 0;
}

int InputManager::handleBluetoothMessage(InputState * state, BluetoothMessage* message)
{
	LOG(LogLevel::Finest) << "InputManager::handleBluetoothCommand : handling bt command " << message << ".";
	return propagateBluetoothMessage(&triggerQueue, state, message);
}

int InputManager::propagateBluetoothMessage(vector<Triggerable*>* queue, InputState * state, BluetoothMessage* message)
{
	LOG(LogLevel::Finest) << "InputManager::propagateBluetoothCommand : handling bt command " << message << ".";

	for (int i = 0; i < queue->size(); i++) {
		LOG(LogLevel::Depricated) << "InputManager::propagateBluetoothCommand : [" << queue->at(i)->GetTypeName() << "].";
		queue->at(i)->TriggerOnMessage(state, message);
	}
	return 0;
}

int InputManager::load()
{
	LOG(LogLevel::Info) << "InputManager::load : get host.";

	GameHost * h = GetCache<GameHost>("GameHost");
	if (!h)
		throw runtime_error("int InputManager::load() : GameHost not found in cache.");

	return load(h);
}

int InputManager::load(GameHost * h)
{
	host = h;
	return 0;
}

int InputManager::iterateUpdateInputQueue(Triggerable * temp, vector<Triggerable*>* tQueue)
{
	int size = tQueue->size();

	vector<ChildAddable*> childs;

	unique_lock<mutex> uLock(ChildMutex);
	childs.assign(temp->GetChilds()->begin(), temp->GetChilds()->end());
	uLock.unlock();

	for (int i = 0; i < childs.size(); i++) {

		Triggerable* tempChild = childs[i]->Cast<Triggerable>();

		if (tempChild->GetIsInputable())
			tQueue->push_back(tempChild);
		iterateUpdateInputQueue(tempChild, tQueue);
	}

	return size == tQueue->size() ? 0 : -1;
}
