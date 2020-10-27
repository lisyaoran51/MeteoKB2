#include "InputHandler.h"

#include "../Commands/BluetoothCommand.h"


using namespace Framework::Input::Handler;
using namespace Framework::Input::Commands;


vector<InputState*>* InputHandler::GetPendingStates()
{
	if (pendingStates.size() > 0)
	if (pendingStates[0]->GetBluetoothState() != nullptr) {
		LOG(LogLevel::Depricated) << "InputHandler::GetPendingStates() : has pending bt input.";
		LOG(LogLevel::Depricated) << "InputHandler::GetPendingStates() : has pending bt input [" << pendingStates[0]->GetBluetoothState()->GetCommands()->at(0)->GetContext().dump() << "].";
	}

	vector<InputState*>* states = new vector<InputState*>();
	states->assign(pendingStates.begin(), pendingStates.end());
	pendingStates.clear();
	return states;
}
