#include "AudioManager.h"


using namespace Framework::Audio;





AudioManager::AudioManager(ResourceStore<char*>* trackStore, ResourceStore<char*>* sampleStore)
{

	trackStore->AddExtension("mp3");

	sampleStore->AddExtension("wav");
	sampleStore->AddExtension("mp3");

	trackManager = GetTrackManager(trackStore);
	sampleManager = GetSampleManager(sampleStore);

	audioThread = new GameThread(bind(&AudioManager::update, this), "AudioThread");

}

SampleManager * AudioManager::GetSampleManager(ResourceStore<char*>* sampleStore)
{
	if (sampleStore == nullptr)
		return sampleManager;

	SampleManager* sManager = new SampleManager(sampleStore);
	AddItem(sManager);
	// TODO: �[�Wadjistment 

	return sManager;
}

TrackManager * AudioManager::GetTrackManager(ResourceStore<char*>* trackStore)
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
