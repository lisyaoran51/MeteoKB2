#include "SoundSelectPanel.h"

#include "../../Instruments/Piano.h"
#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"

using namespace Games::UI;
using namespace Instruments;
using namespace Games::Output::Bluetooths;



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

	
	GetScheduler()->AddDelayedTask([=]() {
	
	
		LOG(LogLevel::Debug) << "SoundSelectPanel::load() : test switch sound select panel on [" << instrument->GetTypeName() << "].";
		/* 測試用，之後要刪掉改用bluetooth */
		TSoundBindingSet<Pitch>* soundBindingSet = dynamic_cast<TSoundBindingSet<Pitch>*>(audioManager->GetSampleManager()->GetSoundBindingSets()->at(0));
		dynamic_cast<Piano*>(instrument)->SwitchSoundBindings(soundBindingSet);
	
		return 0;
	}, 3);

	return 0;
}

SoundSelectPanel::SoundSelectPanel() : RegisterType("SoundSelectPanel")
{
	isInputable = true;

	registerLoad(bind(static_cast<int(SoundSelectPanel::*)(void)>(&SoundSelectPanel::load), this));
}

int SoundSelectPanel::OnCommand(MeteoBluetoothCommand * command)
{

	if (command->GetCommand() == MeteoCommand::AppSwitchPianoInstrument) {

		string soundBank = command->GetContext()["Instrument"];
		
		vector<SoundBindingSet*>* soundBindingSets = audioManager->GetSampleManager()->GetSoundBindingSets();

		SoundBindingSet* soundBindingSet = nullptr;

		for (int i = 0; i < soundBindingSets->size(); i++) {
			if (soundBindingSets->at(i)->fileName == soundBank) {
				soundBindingSet = soundBindingSets->at(i);
			}
		}

		MeteoContextBluetoothMessage* outputMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckAppSwitchPianoInstrument);

		if (soundBindingSet) {
			outputMessage->GetContext()["Status"] = 0;
			outputManager->PushMessage(outputMessage);

			dynamic_cast<Piano*>(instrument)->SwitchSoundBindings(dynamic_cast<TSoundBindingSet<Pitch>*>(soundBindingSet));

			outputMessage = new MeteoContextBluetoothMessage(MeteoCommand::FinishAppSwitchPianoInstrument);
			outputMessage->GetContext()["Instrument"] = soundBank;
			outputManager->PushMessage(outputMessage);

		}
		else {
			outputMessage->GetContext()["Status"] = -1;
			outputManager->PushMessage(outputMessage);
		}


	}

	return 0;
}

int SoundSelectPanel::update()
{
	if (!isFirstUpdate)
		return 0;

	//return 0;

	return 0;
}
