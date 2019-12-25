#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H


#include "../OutputCollectionManager.h"

using namespace Framework::Output;


namespace Framework {
namespace Output{
namespace Bluetooths{

	class BluetoothManager : public OutputCollectionManager<OutputComponent> {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface){}

	};

}}}





#endif
