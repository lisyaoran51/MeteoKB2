#include "MeteoBluetoothDevice.h"

using namespace Desktop::Devices;


MeteoBluetoothDevice::MeteoBluetoothDevice(MeteoBluetoothPhoneV1 * mBluetoothPhone)
{
	meteoBluetoothPhone = mBluetoothPhone;
}

int MeteoBluetoothDevice::readFromDevice()
{
	return 0;
}

int MeteoBluetoothDevice::passToDevice()
{
	for (int i = 0; i < outputMessages.size(); i++) {
		LOG(LogLevel::Depricated) << "MeteoPanelDevice::passToDevice() : pass message to board.";
		meteoBluetoothPhone->PushOutputMessage(outputMessages[i]);
		delete outputMessages[i];
	}
	outputMessages.clear();

	return 0;
}
