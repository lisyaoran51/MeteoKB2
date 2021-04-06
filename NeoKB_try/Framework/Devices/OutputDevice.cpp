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

	unique_lock<mutex> uLock(outputMessageMutex);
	matchedPeripheral->PourOutOutputMessages(&outputMessages);
	return 0;
}
