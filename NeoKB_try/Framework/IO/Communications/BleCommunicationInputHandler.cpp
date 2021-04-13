#include "BleCommunicationInputHandler.h"

#include "CommunicationAccess.h"
#include "../BluetoothPhone.h"

using namespace Framework::IO::Communications;
using namespace Framework::IO;


BleCommunicationInputHandler::BleCommunicationInputHandler(CommunicationAccess * cAccess)
{
	communicationAccess = cAccess;
}

int BleCommunicationInputHandler::Initialize(GameHost * host)
{
	LOG(LogLevel::Info) << "BleCommunicationInputHandler::Initialize() : adding handler.";

	//vector<pair<uintptr_t, string>>* callbackKeys = host->GetMainInterface()->GetKeyboard()->_DebugGetActionList()->_DebugGetCallbackKeys();
	//
	//LOG(LogLevel::Depricated) << "TestCaseKeyboard : callback keys size = " << callbackKeys->size();
	// 當mian interface讀到輸入時，就會call handle key down來處理
	host->GetMainInterface()->GetBluetoothPhone()->
		AddOnRawMessage<BleCommunicationInputHandler>(this, bind((int(BleCommunicationInputHandler::*)(InputState*))&BleCommunicationInputHandler::HandleState, this, placeholders::_1), "BleCommunicationInputHandler::HandleState");


}

int BleCommunicationInputHandler::HandleState(InputState * inputEvent)
{
	if (inputEvent->GetBluetoothState()->CheckIsEmpty())
		return 0;

	communicationAccess->HandleState(inputEvent);

	return 0;
}
