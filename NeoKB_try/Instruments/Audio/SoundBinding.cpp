#include "SoundBinding.h"


using namespace Instruments::Audio;


SoundBinding::SoundBinding(string fName, int a)
{
	fileName = fName;
	action = a;
}

string SoundBinding::GetFileName()
{
	return fileName;
}
