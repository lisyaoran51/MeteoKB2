#include "BluetoothOutputer.h"


using namespace Framework::Output::Bluetooths;


int BluetoothOutputer::SetupPeripheral(MainInterface * mainInterface)
{

	bluetoothPhone = mainInterface->GetBluetoothPhone();


	return 0;
}
