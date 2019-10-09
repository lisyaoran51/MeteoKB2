#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "../Framework/Allocation/Hierachal/Container.h"
#include "Audio\SoundBinding.h"
#include <map>
#include "../Framework/Audio/Sample/SampleChannel.h"
#include "../Framework/Input/KeyBindings/KeyBinding.h"

using namespace Framework::Allocation::Hierachal;
using namespace Instruments::Audio;
using namespace std;
using namespace Framework::Audio::Samples;
using namespace Framework::Input::KeyBindings;

namespace Instruments {


	class Instrument : public Container {


	public:

		virtual vector<SoundBinding*>* GetDefaultSoundBindings() = 0;

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0) = 0;

	protected:

		virtual int LoadOnCompleted();

		map<int, SampleChannel*> samples;

		virtual int onKeyDown(InputState* inputState, InputKey key);

		virtual int onKeyUp(InputState* inputState, InputKey key);

		virtual int onKnobTurn(InputState* inputState, InputKey knob);

		virtual int onButtonDown(InputState* inputState, InputKey button);

		virtual int onButtonUp(InputState* inputState, InputKey button);

		virtual int onSlide(InputState* inputState, InputKey slider);

	};


}




#endif