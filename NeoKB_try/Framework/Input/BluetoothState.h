#ifndef BLUETOOTH_STATE_H
#define BLUETOOTH_STATE_H

#include "PeripheralState.h"
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

namespace Framework {
namespace Input {

	struct BluetoothCommand;

	class BluetoothState : public PeripheralState<BluetoothState> {

		vector<BluetoothCommand*> commands;

	public:

		~BluetoothState();

		int AddCommand(BluetoothCommand* command);

		vector<BluetoothCommand*>* GetCommands();

		virtual BluetoothState* Clone();
	};


	struct BluetoothCommand {

		string CommandType;

	};

}}





#endif