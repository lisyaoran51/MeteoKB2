#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "../Framework/Allocation/Hierachal/Container.h"
#include "Audio\SoundBinding.h"
#include <map>
#include "../Framework/Audio/Sample/SampleChannel.h"
#include "../Framework/Input/KeyBindings/KeyBinding.h"
#include "../Framework/Audio/AudioManager.h"
#include "../Framework/Input/KeyBindings/KeyBindingHandler.h"

using namespace Framework::Allocation::Hierachal;
using namespace Instruments::Audio;
using namespace std;
using namespace Framework::Audio::Samples;
using namespace Framework::Input::KeyBindings;
using namespace Framework::Audio;

namespace Instruments {

	template<typename T>
	class Instrument : public Container, public KeyBindingHandler<T> {

		int load();

		int load(AudioManager* audioManager);

	public:

		Instrument();

		virtual vector<SoundBinding*>* GetDefaultSoundBindings(int variant = 0) = 0;

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0) = 0;

	protected:

		AudioManager* audioManager;

		vector<SoundBinding*> soundBindings;

		map<T, SampleChannel*> samples;

		virtual int LoadOnCompleted();

		virtual int loadAndMapSamples() = 0;

		virtual map<T, SampleChannel*>* getSamples();

		string getSoundBinding(int action);

	};



}




#endif