#include "ReverbGradientTimbreSimpleSoundBindingSet.h"


using namespace Instruments::Audio;


ReverbGradientTimbreSimpleSoundBindingSet::ReverbGradientTimbreSimpleSoundBindingSet()
{
}

ReverbGradientTimbreSimpleSoundBindingSet::ReverbGradientTimbreSimpleSoundBindingSet(string fName) : GradientTimbreSimpleSoundBindingSet(fName)
{
}

GradientTimbreSimpleSoundBinding<Pitch>* ReverbGradientTimbreSimpleSoundBindingSet::GetSoundBinding(Pitch p)
{
	if (p < startKey || p > endKey) {
		// TODO: log error
		return nullptr;
	}

	GradientTimbreSimpleSoundBinding<Pitch>* soundBinding;
	if (isApplyReverb) {
		soundBinding = new ReverbGradientTimbreSimpleSoundBinding<Pitch>(fileName, p);
	}
	else {
		soundBinding = new GradientTimbreSimpleSoundBinding<Pitch>(fileName, p);
	}

	soundBinding->SetTimbreRange(gradientTimbreStartVolume, gradientTimbreEndVolume);

	return soundBinding;
}

int ReverbGradientTimbreSimpleSoundBindingSet::SetIsApplyReverb(bool value)
{
	isApplyReverb = value;
	return 0;
}
