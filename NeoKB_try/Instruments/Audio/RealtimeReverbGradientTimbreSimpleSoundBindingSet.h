#ifndef REALTIME_REVERB_GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_SET_H
#define REALTIME_REVERB_GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_SET_H




#include "RealtimeReverbGradientTimbreSimpleSoundBinding.h"
#include "GradientTimbreSimpleSoundBindingSet.h"



namespace Instruments {
namespace Audio {

	class RealtimeReverbGradientTimbreSimpleSoundBindingSet : public GradientTimbreSimpleSoundBindingSet {

	public:

		RealtimeReverbGradientTimbreSimpleSoundBindingSet();
		RealtimeReverbGradientTimbreSimpleSoundBindingSet(string fName);

		virtual GradientTimbreSimpleSoundBinding<Pitch>* GetSoundBinding(Pitch p);

	protected:

	};

}}









#endif