#ifndef SIMPLE_SOUND_BINDING_H
#define SIMPLE_SOUND_BINDING_H


#include "SoundBinding.h"


namespace Instruments {
namespace Audio {
	
	template<typename T>
	class SimpleSoundBinding : public TSoundBinding<T> {

	public:

		SimpleSoundBinding(string fName, T a): TSoundBinding<T>(fName, a){

		}

		virtual string GetFileName() {
			return SoundBinding::fileName + string("_") + to_string(SoundBinding::action);
		}

	};

}}



#endif