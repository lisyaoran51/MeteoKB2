#include "RepeatSoundBindingSet.h"


using namespace Instruments::Audio;


RepeatSoundBindingSet::RepeatSoundBindingSet()
{
	startKey = Pitch::A2;
	endKey = Pitch::c5;
}

RepeatSoundBindingSet::RepeatSoundBindingSet(string fName, float tlength): TSoundBindingSet(fName)
{
	startKey = Pitch::A2;
	endKey = Pitch::c5;

	tailLength = tlength;
}

string RepeatSoundBindingSet::GetFileName(Pitch p)
{
	return SoundBindingSet::fileName + string("_") + to_string(int(p));
}

RepeatSoundBinding<Pitch>* RepeatSoundBindingSet::GetSoundBinding(Pitch p)
{
	return new RepeatSoundBinding<Pitch>(SoundBindingSet::fileName, p, tailLength);
}
