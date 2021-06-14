#include "SimpleSoundBindingSet.h"


using namespace Instruments::Audio;


SimpleSoundBindingSet::SimpleSoundBindingSet()
{
	startKey = Pitch::A2;
	endKey = Pitch::c5;
}

SimpleSoundBindingSet::SimpleSoundBindingSet(string fName) : TSoundBindingSet<Pitch>(fName)
{
	startKey = Pitch::A2;
	endKey = Pitch::c5;
}

string SimpleSoundBindingSet::GetFileName(Pitch p)
{
	return fileName + string("_") + to_string(int(p));
}

SimpleSoundBinding<Pitch>* SimpleSoundBindingSet::GetSoundBinding(Pitch p)
{
	return new SimpleSoundBinding<Pitch>(fileName, p);
}

int SimpleSoundBindingSet::SetKeyRange(Pitch sKey, Pitch eKey)
{
	startKey = sKey;
	endKey = eKey;
	return 0;
}
