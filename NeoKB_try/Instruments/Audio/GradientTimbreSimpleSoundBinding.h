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
		/// ���q�W�L0��A�N�}�l�Npan�q-1���W��
		/// </summary>
		float gradientTimbreStartVolume = 0;

		/// <summary>
		/// ���q�W�L0��A�N�}�l�Npan�q-1���W�աA���쭵�q��1�ɡApan�զ�1
		/// </summary>
		float gradientTimbreEndVolume = 1;

		int SetTimbreRange(float start, float end) {
			if (gradientTimbreStartVolume > gradientTimbreEndVolume)
				// TODO: ��log
				return -1;
			if (gradientTimbreStartVolume >= 0)
				gradientTimbreStartVolume = start;
			if (gradientTimbreEndVolume <= 1)
				gradientTimbreEndVolume = end;
		}



	};

}}



#endif