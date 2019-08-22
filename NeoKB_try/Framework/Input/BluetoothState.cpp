#include "BluetoothState.h"


using namespace Framework::Input;
using namespace std;


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
