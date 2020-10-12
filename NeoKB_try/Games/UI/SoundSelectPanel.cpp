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
	LOG(LogLevel::Info) << "SoundSelectPanel::load() : setting sound select panel.";

	frameworkConfigManager = f;
	instrument = i;
	audioManager = a;
	outputManager = o;


	return 0;
}

SoundSelectPanel::SoundSelectPanel() : RegisterType("SoundSelectPanel")
{
	isInputable = true;

	registerLoad(bind(static_cast<int(SoundSelectPanel::*)(void)>(&SoundSelectPanel::load), this));
}

int SoundSelectPanel::OnCommand(MeteoBluetoothCommand * command)
{
	return 0;
}

int SoundSelectPanel::update()
{
	if (!isFirstUpdate)
		return 0;

	return 0;

	LOG(LogLevel::Debug) << "SoundSelectPanel::update() : test switch sound select panel.";
	/* 測試用，之後要刪掉改用bluetooth */
	TSoundBindingSet<Pitch>* soundBindingSet = dynamic_cast<TSoundBindingSet<Pitch>*>(audioManager->GetSampleManager()->GetSoundBindingSets()->at(0));
	dynamic_cast<Piano*>(instrument)->SwitchSoundBindings(soundBindingSet);

	return 0;
}
