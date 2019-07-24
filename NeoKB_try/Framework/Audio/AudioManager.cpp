#include "AudioManager.h"


using namespace Framework::Audio;





AudioManager::AudioManager(ResourceStore<string>* trackStore, ResourceStore<string>* sampleStore): RegisterType("AudioManager")
{

	trackStore->AddExtension("mp3");

	sampleStore->AddExtension("wav");
	sampleStore->AddExtension("mp3");

	trackManager = GetTrackManager(trackStore);
	sampleManager = GetSampleManager(sampleStore);

	audioThread = new GameThread(bind(&AudioManager::update, this), "AudioThread");

}

SampleManager * AudioManager::GetSampleManager(ResourceStore<string>* sampleStore)
{
	if (sampleStore == nullptr)
		return sampleManager;

	SampleManager* sManager = new SampleManager(sampleStore);
	AddItem(sManager);
	// TODO: 加上adjistment 

	return sManager;
}

TrackManager * AudioManager::GetTrackManager(ResourceStore<string>* trackStore)
{
	if (trackStore == nullptr)
		return trackManager;

	TrackManager* tManager = new TrackManager(trackStore);
	// TODO: 加上adjistment

	return tManager;
}

GameThread * AudioManager::GetAudioThread()
{
	return audioThread;
}
