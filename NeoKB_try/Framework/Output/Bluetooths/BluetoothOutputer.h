#ifndef BLUETOOTH_OUTPUTER_H
#define BLUETOOTH_OUTPUTER_H


#include "../OutputComponent.h"

using namespace Framework::Output;


namespace Framework {
namespace Output{
namespace Bluetooths{

	class BluetoothOutputer : virtual public OutputComponent {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface);

	protected:

		BluetoothPhone* bluetoothPhone = nullptr;


	};

	template<typename T>
	class TBluetoothOutputer : public TOutputComponent<T>, public BluetoothOutputer {



	};

}}}





#endif
