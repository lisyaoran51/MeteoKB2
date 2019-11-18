#include "Display.h"

#include "../Devices/DisplayDevice.h"


using namespace Framework::IO;
using namespace Framework::Devices;


Display::Display()
{
}

int Display::SetDevice(Device * device)
{
	Peripheral::SetDevice(device);
	matchedDisplayDevice = dynamic_cast<DisplayDevice*>(device);
	LOG(LogLevel::Info) << "Display::SetDevice() : Device = " << device << ", matched display device = " << matchedDisplayDevice;

	return 0;
}

int Display::TriggerOnInput()
{
	// no-op
	return 0;
}

int Display::Show(Map * m)
{
	return matchedDisplayDevice->Show(m);
}
