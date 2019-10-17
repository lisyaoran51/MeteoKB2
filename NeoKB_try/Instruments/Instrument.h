#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "../Framework/Allocation/Hierachal/Container.h"
#include "Audio\SoundBinding.h"
#include <map>
#include "../Framework/Audio/Sample/SampleChannel.h"
#include "../Framework/Input/KeyBindings/KeyBinding.h"
#include "../Framework/Audio/AudioManager.h"
#include "../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../Framework/Audio/Sample/SampleChannel.h"
#include "../Framework/Input/PassThroughInputManager.h"
#include "../Framework/Host/GameHost.h"


using namespace Framework::Allocation::Hierachal;
using namespace Instruments::Audio;
using namespace std;
using namespace Framework::Audio::Samples;
using namespace Framework::Input::KeyBindings;
using namespace Framework::Audio;
using namespace Framework::Audio::Samples;
using namespace Framework::Input;
using namespace Framework::Host;


namespace Framework {
namespace Host {

	//class GameHost;

}}


namespace Instruments {


	class Instrument : public Container {

		int load();

		int load(AudioManager* audioManager);

	public:

		Instrument();

		virtual PassThroughInputManager* CreateInputManager() = 0;

		virtual vector<SoundBinding*>* GetDefaultSoundBindings(int variant = 0) = 0;

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0) = 0;

		int SetHost(GameHost* h);

	protected:

		GameHost* host;

		AudioManager* audioManager;

		vector<SoundBinding*> soundBindings;

		virtual int LoadOnCompleted();

		virtual int loadAndMapSamples() = 0;

		string getSoundBinding(int action);

	};

	template<typename T>
	class TInstrument : public Instrument, public KeyBindingHandler<T> {


	public:

		TInstrument() : RegisterType("TInstrument")
		{
			isInputable = true;
		}

	protected:

		map<T, SampleChannel*> samples;

		virtual map<T, SampleChannel*>* getSamples() {
			return &samples;
		}

	};


}




#endif