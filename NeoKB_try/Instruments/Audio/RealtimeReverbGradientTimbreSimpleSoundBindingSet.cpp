#include "RealtimeReverbGradientTimbreSimpleSoundBindingSet.h"


using namespace Instruments::Audio;


RealtimeReverbGradientTimbreSimpleSoundBindingSet::RealtimeReverbGradientTimbreSimpleSoundBindingSet()
{
}

RealtimeReverbGradientTimbreSimpleSoundBindingSet::RealtimeReverbGradientTimbreSimpleSoundBindingSet(string fName): GradientTimbreSimpleSoundBindingSet(fName)
{
}

GradientTimbreSimpleSoundBinding<Pitch>* RealtimeReverbGradientTimbreSimpleSoundBindingSet::GetSoundBinding(Pitch p)
{
	if (p < startKey || p > endKey) {
		// TODO: log error
		return nullptr;
	}

	GradientTimbreSimpleSoundBinding<Pitch>* soundBinding;
	if (isApplyReverb) {
		soundBinding = new RealtimeReverbGradientTimbreSimpleSoundBinding<Pitch>(fileName, p);
	}
	else {
		soundBinding = new GradientTimbreSimpleSoundBinding<Pitch>(fileName, p);
	}

	soundBinding->SetTimbreRange(gradientTimbreStartVolume, gradientTimbreEndVolume);

	return soundBinding;
}
