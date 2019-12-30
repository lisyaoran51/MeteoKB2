#include "OutputDevice.h"

using namespace Framework::Devices;


int OutputDevice::ProcessOutput()
{
	LOG(LogLevel::Debug) << "OutputDevice::ProcessOutput() : test.";

	readFromPeripheral();
	passToDevice();

	return 0;
}

int OutputDevice::readFromPeripheral()
{
	matchedPeripheral->PourOutOutputMessages(&outputMessages);
	return 0;
}
