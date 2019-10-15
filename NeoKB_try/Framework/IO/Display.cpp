#include "Display.h"


using namespace Framework::IO;


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
	return matchedDisplayDevice->Show(m);
}
