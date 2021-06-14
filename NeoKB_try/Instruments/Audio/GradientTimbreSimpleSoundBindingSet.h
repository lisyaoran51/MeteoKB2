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
		/// ���q�W�L0��A�N�}�l�Npan�q-1���W��
		/// </summary>
		float gradientTimbreStartVolume = 0;

		/// <summary>
		/// ���q�W�L0��A�N�}�l�Npan�q-1���W�աA���쭵�q��1�ɡApan�զ�1
		/// </summary>
		float gradientTimbreEndVolume = 1;

		int SetTimbreRange(float start, float end);


	protected:



	};

}}









#endif