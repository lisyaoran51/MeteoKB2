#ifndef BLUETOOTH_STATE_H
#define BLUETOOTH_STATE_H

#include "PeripheralState.h"
#include <vector>

using namespace std;

namespace Framework {
namespace Input {

	class BluetoothState : public PeripheralState<BluetoothState> {

		vector<BluetoothCommand*> commands;

	public:

		int AddCommand(BluetoothCommand* command);

		vector<BluetoothCommand*>* GetCommands();

		virtual BluetoothState* Clone();
	};


	struct BluetoothCommand {

		string CommandType;

	};

}}





#endif