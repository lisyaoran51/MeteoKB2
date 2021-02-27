#include "Instrument.h"

using namespace Instruments;



int Instrument::load()
{
	AudioManager * a = GetCache<AudioManager>("AudioManager");
	if (!a)
		throw runtime_error("int Instrument::load() : AudioManager not found in cache.");

	return load(a);
}

int Instrument::load(AudioManager * a)
{
	isPresent = true;

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

int Instrument::LoadAndMapSamples()
{
	return loadAndMapSamples();
}

int Instrument::LoadAndMapPitches()
{
	return loadAndMapPitches();
}

int Instrument::Sleep()
{
	if (!isSleeping)
		isSleeping = true;

	return 0;
}

int Instrument::WakeUp()
{
	if (isSleeping)
		isSleeping = true;

	return 0;
}

int Instrument::LoadOnComplete()
{
	LOG(LogLevel::Fine) << "Instrument::LoadOnComplete() : start loading...";

	// 把sound binding丟進去
	// TODO: variant應該要從setting拿
	vector<SoundBinding*>* defaultSoundBindings = GetDefaultSoundBindings(0);
	soundBindings.reserve(defaultSoundBindings->size());
	soundBindings.insert(soundBindings.end(), defaultSoundBindings->begin(), defaultSoundBindings->end());

	delete defaultSoundBindings;


	loadAndMapSamples();

	loadAndMapPitches();

	//寫錯了
	/*
	vector<PitchBinding*>* defaultPitchBindings = GetDefaultPitchBindings();
	pitchBindings.reserve(defaultPitchBindings->size());
	pitchBindings.insert(pitchBindings.end(), defaultPitchBindings->begin(), defaultPitchBindings->end());

	delete defaultPitchBindings;

	mapActionToPitch();
	*/

	return 0;
}

string Instrument::getSoundBindingFileName(int action)
{
	for (int i = 0; i < soundBindings.size(); i++) {
		if (soundBindings[i]->action == action)
			return soundBindings[i]->GetFileName();
	}
	return string();
}

SoundBinding * Instrument::getSoundBinding(int action)
{
	for (int i = 0; i < soundBindings.size(); i++) {
		if (soundBindings[i]->action == action)
			return soundBindings[i];
	}
	LOG(LogLevel::Error) << "Instrument::getSoundBinding() : sound binding [" << action << "] not foung.";

	/*
	LOG(LogLevel::Debug) << [=]() {
		for (int i = 0; i < soundBindings.size(); i++) {
			LOG(LogLevel::Debug) << soundBindings[i]->action << " : " << soundBindings[i]->fileName;
		}
		return 0;
	};
	*/

	return nullptr;
}
