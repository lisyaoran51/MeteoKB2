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
	return SoundBindingSet::fileName + string("_") + to_string(int(p));
}

SimpleSoundBinding<Pitch>* SimpleSoundBindingSet::GetSoundBinding(Pitch p)
{
	return new SimpleSoundBinding<Pitch>(SoundBindingSet::fileName, p);
}
