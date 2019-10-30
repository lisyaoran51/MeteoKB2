#include "AudioManager.h"


using namespace Framework::Audio;





AudioManager::AudioManager(CompositeResourceStore<char*>* trackStore, CompositeResourceStore<char*>* sampleStore): RegisterType("AudioManager")
{

	trackStore->AddExtension("mp3");

	sampleStore->AddExtension("wav");
	sampleStore->AddExtension("mp3");

	trackManager = GetTrackManager(trackStore);
	sampleManager = GetSampleManager(sampleStore);

	audioThread = new GameThread(bind(&AudioManager::update, this), "AudioThread");

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
	// TODO: �[�Wadjistment

	return tManager;
}

GameThread * AudioManager::GetAudioThread()
{
	return audioThread;
}
