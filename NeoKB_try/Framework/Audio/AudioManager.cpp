#include "AudioManager.h"


using namespace Framework::Audio;





AudioManager::AudioManager(CompositeResourceStore<char*>* trackStore, CompositeResourceStore<char*>* sampleStore): RegisterType("AudioManager")
{

	trackStore->AddExtension("mp3");

	sampleStore->AddExtension("wav");
	sampleStore->AddExtension("mp3");

	trackManager = GetTrackManager(trackStore);
	sampleManager = GetSampleManager(sampleStore);

	audioThread = new GameThread(bind(&AudioManager::Update, this), "AudioThread");

}

SampleManager * AudioManager::GetSampleManager(CompositeResourceStore<char*>* sampleStore)
{
	if (sampleStore == nullptr)
		return sampleManager;

	SampleManager* sManager = new SampleManager(sampleStore);
	AddItem(sManager);
	// TODO: 加上adjistment 

	return sManager;
}

TrackManager * AudioManager::GetTrackManager(CompositeResourceStore<char*>* trackStore)
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
