#include "BluetoothState.h"


using namespace Framework::Input;
using namespace std;


BluetoothState::~BluetoothState()
{
	for (int i = 0; i < commands.size(); i++)
		delete commands[i];
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
	cloned->SetLastState(lastState);
	cloned->GetCommands()->assign(commands.begin(), commands.end());

	return cloned;
}

BluetoothCommand::BluetoothCommand(BluetoothCommand * copied)
{
	CommandType = copied->CommandType;
}
