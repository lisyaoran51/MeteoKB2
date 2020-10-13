#ifndef REPEAT_SOUND_BINDING_H
#define REPEAT_SOUND_BINDING_H


#include "SoundBinding.h"


namespace Instruments {
namespace Audio {
	
	template<typename T>
	class RepeatSoundBinding : public TSoundBinding<T> {

	public:

		RepeatSoundBinding(string sBankName, T a, float tLength) : TSoundBinding<T>(sBankName, a) {
			tailLength = tLength;
		}

		float tailLength = 10.f;

	};

}}



#endif