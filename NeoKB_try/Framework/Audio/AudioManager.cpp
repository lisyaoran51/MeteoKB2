#include "AudioManager.h"

#include "../../ThirdParty/Bass/bass.h"


using namespace Framework::Audio;





AudioManager::AudioManager(CompositeResourceStore<char*>* trackStore, CompositeResourceStore<char*>* sampleStore): RegisterType("AudioManager")
{

	trackStore->AddExtension("mp3");

	sampleStore->AddExtension("wav");
	sampleStore->AddExtension("mp3");

	trackManager = GetTrackManager(trackStore);
	sampleManager = GetSampleManager(sampleStore);

	if (!BASS_Init(-1, 44100, 0, 0, NULL))
		throw runtime_error("AudioManager::AudioManager() :cannot initialize bass.");

	audioThread = new GameThread(bind(&AudioManager::Update, this), "AudioThread");
	audioThread->SetMaxUpdateHz(1000);
	audioThread->Start();
	clock = audioThread->GetClock();
}

SampleManager * AudioManager::GetSampleManager(CompositeResourceStore<char*>* sampleStore)
{
	LOG(LogLevel::Depricated) << "AudioManager::GetSampleManager() : get sample manager [" << sampleManager << "].";
	if (sampleStore == nullptr)
		return sampleManager;


	SampleManager* sManager = new SampleManager(sampleStore);
	AddItem(sManager);

	sampleVolume->SetValue(1.0);
	sManager->AddAdjustment(AdjustableProperty::Volume, sampleVolume);
	// TODO: 加上adjistment 

	return sManager;
}

TrackManager * AudioManager::GetTrackManager(CompositeResourceStore<char*>* trackStore)
{
	if (trackStore == nullptr)
		return trackManager;

	TrackManager* tManager = new TrackManager(trackStore);
	AddItem(tManager);
	tManager->AddAdjustment(AdjustableProperty::Volume, trackVolume);
	
	trackVolume->SetValue(0.3);
	// TODO: 加上adjistment

	return tManager;
}

GameThread * AudioManager::GetAudioThread()
{
	return audioThread;
}

Bindable<double>* AudioManager::GetTrackVolume() {
	return trackVolume;
}

Bindable<double>* AudioManager::GetSampleVolume() {
	return sampleVolume;
}