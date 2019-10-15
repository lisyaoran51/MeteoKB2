#include "Instrument.h"

using namespace Instruments;







template<typename T>
TInstrument<T>::TInstrument() : RegisterType("TInstrument")
{
}

template<typename T>
map<int, SampleChannel*>* TInstrument::getSamples()
{
	return &samples;
}


int Instrument::load()
{
	AudioManager * a = GetCache<AudioManager>("AudioManager");
	if (!a)
		throw runtime_error("int Instrument::load() : AudioManager not found in cache.");

	return load(a);
}

int Instrument::load(AudioManager * a)
{
	audioManager = a;
	return 0;
}

Instrument::Instrument() : RegisterType("Instrument")
{
	registerLoad(bind(static_cast<int(Instrument::*)(void)>(&Instrument::load), this));
}

int Instrument::SetHost(GameHost * h)
{
	host = h;
	return 0;
}

int Instrument::LoadOnCompleted()
{
	// 把sound binding丟進去
	// TODO: variant應該要從setting拿
	vector<SoundBinding*>* defaultSoundBindings = GetDefaultSoundBindings();
	soundBindings.reserve(defaultSoundBindings->size());
	soundBindings.insert(soundBindings.end(), defaultSoundBindings->begin(), defaultSoundBindings->end());

	delete defaultSoundBindings;

	loadAndMapSamples();

	return 0;
}

string Instrument::getSoundBinding(int action)
{
	for (int i = 0; i < soundBindings.size(); i++) {
		if (soundBindings[i]->action == action)
			return soundBindings[i]->fileName;
	}
	return string();
}