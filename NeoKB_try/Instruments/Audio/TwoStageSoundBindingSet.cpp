#include "TwoStageSoundBindingSet.h"


using namespace Instruments::Audio;


TwoStageSoundBindingSet::TwoStageSoundBindingSet()
{
	startKey = Pitch::A2;
	endKey = Pitch::c5;
}

TwoStageSoundBindingSet::TwoStageSoundBindingSet(string fName, float sPoint, float tLength): TSoundBindingSet(fName)
{
	startKey = Pitch::A2;
	endKey = Pitch::c5;

	switchPoint = sPoint;
	tailLength = tLength;
}

string TwoStageSoundBindingSet::GetFileName(Pitch p)
{
	return SoundBindingSet::fileName + string("_") + to_string(int(p));
}

TwoStageSoundBinding<Pitch>* TwoStageSoundBindingSet::GetSoundBinding(Pitch p)
{
	return new TwoStageSoundBinding<Pitch>(SoundBindingSet::fileName, p, switchPoint, tailLength);
}

string TwoStageSoundBindingSet::GetStageAFileName(Pitch p)
{
	return SoundBindingSet::fileName + string("_A_") + to_string(int(p));
}

string TwoStageSoundBindingSet::GetStageBFileName(Pitch p)
{
	return SoundBindingSet::fileName + string("_B_") + to_string(int(p));
}
