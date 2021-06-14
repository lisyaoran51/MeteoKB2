#include "GradientTimbreSimpleSoundBindingSet.h"


using namespace Instruments::Audio;


GradientTimbreSimpleSoundBindingSet::GradientTimbreSimpleSoundBindingSet()
{
}

GradientTimbreSimpleSoundBindingSet::GradientTimbreSimpleSoundBindingSet(string fName): SimpleSoundBindingSet(fName)
{
}

GradientTimbreSimpleSoundBinding<Pitch>* GradientTimbreSimpleSoundBindingSet::GetSoundBinding(Pitch p)
{
	if (p < startKey || p > endKey) {
		// TODO: log error
		return nullptr;
	}

	GradientTimbreSimpleSoundBinding<Pitch>* soundBinding = new GradientTimbreSimpleSoundBinding<Pitch>(fileName, p);
	soundBinding->SetTimbreRange(gradientTimbreStartVolume, gradientTimbreEndVolume);
	return soundBinding;
}

int GradientTimbreSimpleSoundBindingSet::SetTimbreRange(float start, float end) {
	if (gradientTimbreStartVolume > gradientTimbreEndVolume)
		// TODO: ¥álog
		return -1;
	if (gradientTimbreStartVolume >= 0 && gradientTimbreStartVolume <= 1)
		gradientTimbreStartVolume = start;
	if (gradientTimbreEndVolume >= 0 && gradientTimbreEndVolume <= 1)
		gradientTimbreEndVolume = end;
}
