#include "BluetoothState.h"

#include "../../Util/Log.h"


using namespace Framework::Input;
using namespace std;
using namespace Util;


BluetoothState::~BluetoothState()
{
	LOG(LogLevel::Finest) << "BluetoothState::~BluetoothState(): deleting commands." << commands.size();
	for (int i = 0; i < commands.size(); i++) {
		LOG(LogLevel::Finest) << "BluetoothState::~BluetoothState(): deleting command [" << commands[i]->GetContext()["FileName"] << "] .";
		delete commands[i];

	}
	commands.clear();
	LOG(LogLevel::Finest) << "BluetoothState::~BluetoothState(): deleted.";
}

int BluetoothState::AddCommand(BluetoothCommand * command)
{
	commands.push_back(command);
	return 0;
}

vector<BluetoothCommand*>* BluetoothState::GetCommands()
{
	return &commands;
}

BluetoothState * BluetoothState::Clone()
{
	BluetoothState* cloned = new BluetoothState();

	if(lastState != nullptr)
		cloned->SetLastState(lastState);
	cloned->GetCommands()->assign(commands.begin(), commands.end());

	return cloned;
}

bool BluetoothState::CheckIsEmpty()
{
	return commands.size() == 0;
}

