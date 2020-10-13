#include "SoundBinding.h"


using namespace Instruments::Audio;


SoundBinding::SoundBinding(string sBankName, int a)
{
	soundBankName = sBankName;
	action = a;
}

string SoundBinding::GetSoundBankName()
{
	return soundBankName;
}
