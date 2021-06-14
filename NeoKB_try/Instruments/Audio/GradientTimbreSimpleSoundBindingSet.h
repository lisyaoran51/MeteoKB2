#ifndef GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_SET_H
#define GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_SET_H




#include "SimpleSoundBindingSet.h"
#include "GradientTimbreSimpleSoundBinding.h"



namespace Instruments {
namespace Audio {

	class GradientTimbreSimpleSoundBindingSet : public SimpleSoundBindingSet {

	public:

		GradientTimbreSimpleSoundBindingSet();
		GradientTimbreSimpleSoundBindingSet(string fName);

		virtual GradientTimbreSimpleSoundBinding<Pitch>* GetSoundBinding(Pitch p);

		/// <summary>
		/// 當音量超過0後，就開始將pan從-1往上調
		/// </summary>
		float gradientTimbreStartVolume = 0;

		/// <summary>
		/// 當音量超過0後，就開始將pan從-1往上調，直到音量為1時，pan調至1
		/// </summary>
		float gradientTimbreEndVolume = 1;

		int SetTimbreRange(float start, float end);


	protected:



	};

}}









#endif