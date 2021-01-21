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

	LOG(LogLevel::Finest) << "PassThroughInputManager::getPendingState(): [" << GetTypeName() << "] start update." << (useParentState ? 1 : 0) << pendingStates.size();

	if(!useParentState)
		for (int i = 0; i < pendingStates.size(); i++) {	// �qinput handler�ЫءA��o��delete��

			LOG(LogLevel::Finest) << "PassThroughInputManager::getPendingState(): [" << GetTypeName() << "] deleting pending state [" << pendingStates[i] << "].";
			delete pendingStates[i];						//
		}

	pendingStates.clear();

	LOG(LogLevel::Finest) << "PassThroughInputManager::getPendingState(): [" << GetTypeName() << "] update finished.";

	return 0;
}

vector<InputState*>* PassThroughInputManager::getPendingState(vector<InputState*>* pStates)
{
	LOG(LogLevel::Depricated) << "PassThroughInputManager::getPendingState(): " << GetTypeName() << " start getting passing states." << (useParentState ? 1 : 0) ;
	/* ����handlers�̭���pending states�����M�X�� */
	InputManager::getPendingState(pStates);

	if (!useParentState)
		return pStates;

	pStates->clear();

	for (int i = 0; i < pendingParentStates.size(); i++) {

		LOG(LogLevel::Depricated) << "PassThroughInputManager::getPendingState(): " << GetTypeName() << " passing states.";
		// �ܦ��i�ೣ�O���ƪ��P�@��input state�A�ҥH���T�w�@�U
		if (find(pStates->begin(), pStates->end(), pendingParentStates[i]) == pStates->end())
			pStates->push_back(pendingParentStates[i]);

		// �U���o�ӬO����F��? ������State�i�H�୼command?
		//if (dynamic_cast<BluetoothCommand*>(pendingParentStates[i])) {
		//	LOG(LogLevel::Depricated) << "PassThroughInputManager::getPendingState(): push [" << dynamic_cast<BluetoothCommand*>(pendingParentStates[i])->GetContext().dump() << "] into pending states.";
		//}
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

int PassThroughInputManager::onMessage(InputState * inputState, BluetoothMessage * bluetoothMessage)
{
	//LOG(LogLevel::Finest) << "PassThroughInputManager::onMessage(): get command [" << bluetoothMessage->GetContext().dump() << "].";

	return acceptState(inputState);
}
