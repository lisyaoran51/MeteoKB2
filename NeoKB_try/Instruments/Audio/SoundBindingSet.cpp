#include "SoundBindingSet.h"


using namespace Instruments::Audio;



SoundBindingSet::SoundBindingSet(string fName)
{
	fileName = fName;
}

int SoundBindingSet::SetKeySpan(Pitch sKey, Pitch eKey)
{
	startKey = sKey;
	EndKey = eKey;
	return 0;
}

Pitch SoundBindingSet::GetStartKey()
{
	return startKey;
}

Pitch SoundBindingSet::GetEndKey()
{
	return EndKey;
}

string SoundBindingSet::GetFileName(Pitch p)
{
	return string();
}
