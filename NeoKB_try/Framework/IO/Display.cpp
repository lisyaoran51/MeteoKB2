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
	return 0;
}

int Display::TriggerOnInput()
{
	// no-op
	return 0;
}

int Display::Show(Map * m)
{
	LOG(LogLevel::Debug) << "Display::Show() : matched display device = " << matchedDisplayDevice;

	return matchedDisplayDevice->Show(m);
}
