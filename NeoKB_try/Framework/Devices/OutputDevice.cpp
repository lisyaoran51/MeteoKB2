#include "OutputDevice.h"

using namespace Framework::Devices;


int OutputDevice::ProcessOutput()
{

	readFromPeripheral();
	passToDevice();
	LOG(LogLevel::Depricated) << "OutputDevice::ProcessOutput() : process over.";

	return 0;
}

int OutputDevice::readFromPeripheral()
{

	//unique_lock<mutex> uLock(outputMessageMutex);

	if (!outputMessageMutex.try_lock()) {
		LOG(LogLevel::Depricated) << "OutputDevice::readFromPeripheral() : outputMessageMutex locked.";
		return 0;
	}

	matchedPeripheral->PourOutOutputMessages(&outputMessages);

	outputMessageMutex.unlock();
	return 0;
}
