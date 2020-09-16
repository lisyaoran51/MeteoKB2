#include "TwoStageSoundBindingSet.h"


using namespace Instruments::Audio;


TwoStageSoundBindingSet::TwoStageSoundBindingSet(string fName, float sPoint, float tLength): SoundBindingSet(fName)
{
	switchPoint = sPoint;
	tailLength = tLength;
}

float TwoStageSoundBindingSet::GetSwitchPoint()
{
	return switchPoint;
}

float TwoStageSoundBindingSet::GetTailLength()
{
	return tailLength;
}

string TwoStageSoundBindingSet::GetStageAFileName(Pitch p)
{
	return fileName + string("_A_") + to_string(int(p));
}

string TwoStageSoundBindingSet::GetStageBFileName(Pitch p)
{
	return fileName + string("_B_") + to_string(int(p));
}
