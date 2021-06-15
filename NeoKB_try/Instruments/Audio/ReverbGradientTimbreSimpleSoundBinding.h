#ifndef REVERB_GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_H
#define REVERB_GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_H


#include "GradientTimbreSimpleSoundBinding.h"


namespace Instruments {
namespace Audio {
	
	template<typename T>
	class ReverbGradientTimbreSimpleSoundBinding : public GradientTimbreSimpleSoundBinding<T> {

	public:

		ReverbGradientTimbreSimpleSoundBinding(string sBankName, T a): GradientTimbreSimpleSoundBinding<T>(sBankName, a){

		}

		virtual string GetReverbFileName() {
			return SoundBinding::soundBankName + string("_") + to_string(SoundBinding::action) + "_RevHiss"; // ����n�令_Rev
		}
	};

}}



#endif