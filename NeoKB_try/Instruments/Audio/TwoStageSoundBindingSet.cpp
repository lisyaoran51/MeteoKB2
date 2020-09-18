#include "TwoStageSoundBindingSet.h"


using namespace Instruments::Audio;


TwoStageSoundBindingSet::TwoStageSoundBindingSet(string fName, float sPoint, float tLength): TSoundBindingSet(fName)
{
	startKey = Pitch::A2;
	endKey = Pitch::c5;

	switchPoint = sPoint;
	tailLength = tLength;
}

string TwoStageSoundBindingSet::GetFileName(Pitch p)
{
	return "";
}

TwoStageSoundBinding<Pitch>* TwoStageSoundBindingSet::GetSoundBinding(Pitch p)
{
	return new TwoStageSoundBinding<Pitch>(fileName, p, switchPoint, tailLength);
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
