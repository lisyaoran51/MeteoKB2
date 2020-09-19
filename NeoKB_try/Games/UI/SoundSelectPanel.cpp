#include "SoundSelectPanel.h"

#include "../../Instruments/Piano.h"

using namespace Games::UI;
using namespace Instruments;



int SoundSelectPanel::load()
{
	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int SongSelect::load() : FrameworkConfigManager not found in cache.");

	Instrument* i = GetCache<Instrument>("Instrument");
	if (!i)
		throw runtime_error("int SongSelect::load() : Instrument not found in cache.");

	AudioManager* a = GetCache<AudioManager>("AudioManager");
	if (!a)
		throw runtime_error("int SongSelect::load() : AudioManager not found in cache.");

	OutputManager* o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("int SongSelect::load() : OutputManager not found in cache.");

	return load(f, i, a, o);
}

int SoundSelectPanel::load(FrameworkConfigManager * f, Instrument* i, AudioManager* a, OutputManager * o)
{
	frameworkConfigManager = f;
	instrument = i;
	audioManager = a;
	outputManager = o;

	/* 測試用，之後要刪掉改用bluetooth */
	TSoundBindingSet<Pitch>* soundBindingSet = dynamic_cast<TSoundBindingSet<Pitch>*>(audioManager->GetSampleManager()->GetSoundBindingSets()->at(0));
	dynamic_cast<Piano*>(instrument)->SwitchSoundBindings(soundBindingSet);

	return 0;
}

SoundSelectPanel::SoundSelectPanel() : RegisterType("SoundSelectPanel")
{
}

int SoundSelectPanel::OnCommand(MeteoBluetoothCommand * command)
{
	return 0;
}
