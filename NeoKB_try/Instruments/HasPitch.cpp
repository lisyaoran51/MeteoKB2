#include "HasPitch.h"


using namespace Instruments;

HasPitch::HasPitch(Pitch p)
{
	pitch = p;
}

Pitch HasPitch::GetPitch()
{
	return pitch;
}
