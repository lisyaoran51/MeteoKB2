#ifndef BLUETOOTH_OUTPUTER_H
#define BLUETOOTH_OUTPUTER_H


#include "../OutputComponent.h"

using namespace Framework::Output;


namespace Framework {
namespace Output{
namespace Bluetooths{

	class BluetoothOutputer : virtual public OutputComponent {



	};

	template<typename T>
	class TBluetoothOutputer : public TOutputComponent<T>, public BluetoothOutputer {



	};

}}}





#endif
