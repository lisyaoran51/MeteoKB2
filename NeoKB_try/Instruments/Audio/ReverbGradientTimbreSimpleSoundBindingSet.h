#ifndef REVERB_GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_SET_H
#define REVERB_GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_SET_H




#include "ReverbGradientTimbreSimpleSoundBinding.h"
#include "GradientTimbreSimpleSoundBindingSet.h"



namespace Instruments {
namespace Audio {

	class ReverbGradientTimbreSimpleSoundBindingSet : public GradientTimbreSimpleSoundBindingSet {

	public:

		ReverbGradientTimbreSimpleSoundBindingSet();
		ReverbGradientTimbreSimpleSoundBindingSet(string fName);

		virtual GradientTimbreSimpleSoundBinding<Pitch>* GetSoundBinding(Pitch p);

		virtual int SetIsApplyReverb(bool value);

	protected:

		bool isApplyReverb = false;

	};

}}









#endif