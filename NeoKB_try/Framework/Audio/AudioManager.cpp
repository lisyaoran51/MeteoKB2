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
	audioThread->SetMaxUpdateHz(50);
	audioThread->Start();
}

SampleManager * AudioManager::GetSampleManager(CompositeResourceStore<char*>* sampleStore)
{
	if (sampleStore == nullptr)
		return sampleManager;

	SampleManager* sManager = new SampleManager(sampleStore);
	AddItem(sManager);
	// TODO: �[�Wadjistment 

	return sManager;
}

TrackManager * AudioManager::GetTrackManager(CompositeResourceStore<char*>* trackStore)
{
	if (trackStore == nullptr)
		return trackManager;

	TrackManager* tManager = new TrackManager(trackStore);
	AddItem(tManager);
	// TODO: �[�Wadjistment

	return tManager;
}

GameThread * AudioManager::GetAudioThread()
{
	return audioThread;
}
