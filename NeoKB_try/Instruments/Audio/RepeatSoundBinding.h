#ifndef REPEAT_SOUND_BINDING_H
#define REPEAT_SOUND_BINDING_H


#include "SoundBinding.h"


namespace Instruments {
namespace Audio {
	
	template<typename T>
	class RepeatSoundBinding : public TSoundBinding<T> {

	public:

		RepeatSoundBinding(string fName, T a, float tLength) : TSoundBinding<T>(fName, a) {
			tailLength = tLength;
		}

		float tailLength = 10.f;

	};

}}



#endif