#include "Instrument.h"

using namespace Instruments;

template<typename T>
int Instrument::load()
{
	AudioManager * a = GetCache<AudioManager>("AudioManager");
	if (!a)
		throw runtime_error("int Instrument::load() : AudioManager not found in cache.");

	return load(a);
}

template<typename T>
int Instrument::load(AudioManager * a)
{
	audioManager = a;
	return 0;
}

template<typename T>
Instrument::Instrument()
{
	registerLoad(bind(static_cast<int(Instrument<T>::*)(void)>(&Instrument<T>::load), this));
}

template<typename T>
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

template<typename T>
map<int, SampleChannel*>* Instrument::getSamples()
{
	return &samples;
}

template<typename T>
string Instrument::getSoundBinding(int action)
{
	for (int i = 0; i < soundBindings.size(); i++) {
		if (soundBindings[i]->action == action)
			return soundBindings[i]->fileName;
	}
	return string();
}
