#ifndef BLUETOOTH_STATE_H
#define BLUETOOTH_STATE_H

#include "PeripheralState.h"
#include <vector>
#include <string>
#include <stdexcept>
#include "../Output/Bluetooths/BluetoothMessage.h"

using namespace std;
using namespace Framework::Output::Bluetooths;

namespace Framework {
namespace Input {

	class BluetoothState : public PeripheralState<BluetoothState> {

		vector<BluetoothMessage*> messages;

	public:

		~BluetoothState();

		int AddMessage(BluetoothMessage* message);

		vector<BluetoothMessage*>* GetMessages();

		virtual BluetoothState* Clone();

		virtual bool CheckIsEmpty();
	};


	

}}





#endif