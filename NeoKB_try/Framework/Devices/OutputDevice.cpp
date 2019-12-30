#include "OutputDevice.h"

using namespace Framework::Devices;


int OutputDevice::ProcessOutput()
{

	readFromPeripheral();
	passToDevice();

	return 0;
}

int OutputDevice::readFromPeripheral()
{
	LOG(LogLevel::Debug) << "OutputDevice::readFromPeripheral() : read from peripheral.";
	matchedPeripheral->PourOutOutputMessages(&outputMessages);
	return 0;
}
