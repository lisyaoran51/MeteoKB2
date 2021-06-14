#ifndef GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_H
#define GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_H


#include "SimpleSoundBinding.h"


namespace Instruments {
namespace Audio {
	
	template<typename T>
	class GradientTimbreSimpleSoundBinding : public SimpleSoundBinding<T> {

	public:

		GradientTimbreSimpleSoundBinding(string sBankName, T a): SimpleSoundBinding<T>(sBankName, a){

		}

		virtual string GetFileName() {
			return SoundBinding::soundBankName + string("_") + to_string(SoundBinding::action);
		}

		/// <summary>
		/// 當音量超過0後，就開始將pan從-1往上調
		/// </summary>
		float gradientTimbreStartVolume = 0;

		/// <summary>
		/// 當音量超過0後，就開始將pan從-1往上調，直到音量為1時，pan調至1
		/// </summary>
		float gradientTimbreEndVolume = 1;

		int SetTimbreRange(float start, float end) {
			if (gradientTimbreStartVolume > gradientTimbreEndVolume)
				// TODO: 丟log
				return -1;
			if (gradientTimbreStartVolume >= 0)
				gradientTimbreStartVolume = start;
			if (gradientTimbreEndVolume <= 1)
				gradientTimbreEndVolume = end;
		}



	};

}}



#endif