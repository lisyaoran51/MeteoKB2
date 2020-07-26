#ifndef BLUETOOTH_STATE_H
#define BLUETOOTH_STATE_H

#include "PeripheralState.h"
#include <vector>
#include <string>
#include <stdexcept>
#include "Commands/BluetoothCommand.h"

using namespace std;
using namespace Framework::Input::Commands;

namespace Framework {
namespace Input {

	class BluetoothState : public PeripheralState<BluetoothState> {

		vector<BluetoothCommand*> commands;

	public:

		~BluetoothState();

		int AddCommand(BluetoothCommand* command);

		vector<BluetoothCommand*>* GetCommands();

		virtual BluetoothState* Clone();

		virtual bool CheckIsEmpty();
	};


	

}}





#endif