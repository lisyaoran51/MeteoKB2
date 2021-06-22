#include "SoundSelectPanel.h"

#include "../../Instruments/Piano.h"
#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Instruments/Audio/ReverbGradientTimbreSimpleSoundBindingSet.h"
#include "../../Instruments/Configurations/InstrumentConfigManager.h"

using namespace Games::UI;
using namespace Instruments;
using namespace Games::Output::Bluetooths;
using namespace Instruments::Audio;
using namespace Instruments::Configurations;



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

	isPresent = true;

	//return 0;
	GetScheduler()->AddDelayedTask(bind(&SoundSelectPanel::firstLoadSound, this), 0.1);

	return 0;
}

SoundSelectPanel::SoundSelectPanel() : RegisterType("SoundSelectPanel")
{
	isInputable = true;

	registerLoad(bind(static_cast<int(SoundSelectPanel::*)(void)>(&SoundSelectPanel::load), this));
}

int SoundSelectPanel::firstLoadSound()
{
	InstrumentConfigManager* instrumentConfigManager = GetCache<InstrumentConfigManager>("InstrumentConfigManager");
	if (!instrumentConfigManager) {
		LOG(LogLevel::Debug) << "SoundSelectPanel::firstLoadSound() : config and sound binding sets not ready. reload later.";
		GetScheduler()->AddDelayedTask(bind(&SoundSelectPanel::firstLoadSound, this), 0.1);
		return 0;
	}

	string fisrtLoadSoundBankName = "U3";

	instrumentConfigManager->Get(InstrumentSetting::InitialSoundBankName, &fisrtLoadSoundBankName);

	LOG(LogLevel::Debug) << "SoundSelectPanel::firstLoadSound() : first switch sound to [" << fisrtLoadSoundBankName << "].";
	vector<SoundBindingSet*>* soundBindingSets = audioManager->GetSampleManager()->GetSoundBindingSets();

	if (soundBindingSets->size() == 0) {
		LOG(LogLevel::Debug) << "SoundSelectPanel::firstLoadSound() : config and sound binding sets not ready. reload later.";
		GetScheduler()->AddDelayedTask(bind(&SoundSelectPanel::firstLoadSound, this), 0.1);
		return 0;
	}

	for (int i = 0; i < soundBindingSets->size(); i++) {
		if (soundBindingSets->at(i)->fileName == fisrtLoadSoundBankName) {
			dynamic_cast<SimpleSoundBindingSet*>(soundBindingSets->at(i))->SetIsApplyReverb(true);
			dynamic_cast<Piano*>(instrument)->SwitchSoundBindings(dynamic_cast<TSoundBindingSet<Pitch>*>(soundBindingSets->at(i)));
			return 0;
		}
	}

	LOG(LogLevel::Debug) << "SoundSelectPanel::firstLoadSound() : fail to find sound [" << fisrtLoadSoundBankName << "], default use [" << soundBindingSets->at(0)->fileName << "]";
	dynamic_cast<Piano*>(instrument)->SwitchSoundBindings(dynamic_cast<TSoundBindingSet<Pitch>*>(soundBindingSets->at(0)));

	return 0;


}

int SoundSelectPanel::onMessage(MeteoBluetoothMessage * message)
{
	LOG(LogLevel::Depricated) << "SoundSelectPanel::onMessage() : got new bt command. ";
	MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);

	if (message->GetCommand() == MeteoCommand::AppSwitchKeyboardInstrument) {
		LOG(LogLevel::Debug) << "SoundSelectPanel::onMessage() : AppSwitchKeyboardInstrument. ";

		json context = contextMessage->GetContextInJson();

		string soundBank = context["Instrument"];
		
		vector<SoundBindingSet*>* soundBindingSets = audioManager->GetSampleManager()->GetSoundBindingSets();

		SoundBindingSet* soundBindingSet = nullptr;

		for (int i = 0; i < soundBindingSets->size(); i++) {
			if (soundBindingSets->at(i)->fileName == soundBank) {
				soundBindingSet = soundBindingSets->at(i);
			}
		}

		MeteoContextBluetoothMessage* outputMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckAppSwitchKeyboardInstrument);

		json returnContext;

		if (soundBindingSet) {
			returnContext["Status"] = 0;

			dynamic_cast<Piano*>(instrument)->SwitchSoundBindings(dynamic_cast<TSoundBindingSet<Pitch>*>(soundBindingSet));
		}
		else {
			// ¤Á´«¥¢±Ñ
			LOG(LogLevel::Warning) << "SoundSelectPanel::onMessage() : switch instrument failed. ";
			returnContext["Status"] = -1;
		}
		outputMessage->SetContextInJson(returnContext);
		outputMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(outputMessage);


	}
	LOG(LogLevel::Depricated) << "SoundSelectPanel::onMessage() : over. ";

	return 0;
}

int SoundSelectPanel::update()
{
	if (!isFirstUpdate)
		return 0;

	//return 0;

	return 0;
}
