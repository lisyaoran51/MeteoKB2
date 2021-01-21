#include "BluetoothState.h"

#include "../../Util/Log.h"


using namespace Framework::Input;
using namespace std;
using namespace Util;


BluetoothState::~BluetoothState()
{
	LOG(LogLevel::Finest) << "BluetoothState::~BluetoothState(): deleting commands." << messages.size();
	for (int i = 0; i < messages.size(); i++) {
		//LOG(LogLevel::Finest) << "BluetoothState::~BluetoothState(): deleting command [" << messages[i]->GetContext()["FileName"] << "] .";
		delete messages[i];

	}
	messages.clear();
	LOG(LogLevel::Finest) << "BluetoothState::~BluetoothState(): deleted.";
}

int BluetoothState::AddMessage(BluetoothMessage * message)
{
	messages.push_back(message);
	return 0;
}

vector<BluetoothMessage*>* BluetoothState::GetMessages()
{
	return &messages;
}

BluetoothState * BluetoothState::Clone()
{
	BluetoothState* cloned = new BluetoothState();

	if(lastState != nullptr)
		cloned->SetLastState(lastState);

	for (int i = 0; i < messages.size(); i++) {
		cloned->GetMessages()->push_back(messages[i]->Clone());
	}

	return cloned;
}

bool BluetoothState::CheckIsEmpty()
{
	return messages.size() == 0;
}

