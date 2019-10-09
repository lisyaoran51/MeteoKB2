#ifndef PIANO_H
#define PIANO_H

#include "Instrument.h"
#include "../Framework/Input/KeyBindings/KeyBinding.h"

using namespace Framework::Input::KeyBindings;


namespace Instruments {


	class Piano : public Instrument {

	public:

		virtual vector<SoundBinding*>* GetDefaultSoundBindings();

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0);

	};


}




#endif