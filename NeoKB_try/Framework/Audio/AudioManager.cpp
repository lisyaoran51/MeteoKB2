#include "AudioManager.h"

#include "../../ThirdParty/Bass/bass.h"


using namespace Framework::Audio;





AudioManager::AudioManager(CompositeResourceStore<char*>* trackStore, CompositeResourceStore<char*>* sampleStore): RegisterType("AudioManager")
{

	trackStore->AddExtension("mp3");

	sampleStore->AddExtension("wav");
	sampleStore->AddExtension("mp3");

	//trackManager = GetTrackManager(trackStore);
	sampleManager = GetSampleManager(sampleStore);
	mirrorSampleManager = GetMirrorSampleManager(sampleStore);

	if (!BASS_Init(-1, 44100, 0, 0, NULL))
		throw runtime_error("AudioManager::AudioManager() :cannot initialize bass.");

	audioThread = new GameThread(bind(&AudioManager::Update, this), "AudioThread");
	audioThread->SetMaxUpdateHz(500);
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

	/* �o��S��sond binding set���ѭ��q�j�p�A���w�]�o�խ��q�j�p�O0.8 */
	sManager->GetVolume()->SetValue(1.0);

	sampleVolume->SetValue(1.0);
	sManager->AddAdjustment(AdjustableProperty::Volume, sampleVolume);
	sampleVolume->AddOnValueChanged(sManager, bind(&SampleManager::InvalidateState, sManager), "SampleManager::InvalidateState");
	// TODO: �[�Wadjistment 

	return sManager;
}

SampleManager * AudioManager::GetMirrorSampleManager(CompositeResourceStore<char*>* sampleStore)
{
	LOG(LogLevel::Depricated) << "AudioManager::GetSampleManager() : get sample manager [" << sampleManager << "].";
	if (sampleStore == nullptr)
		return mirrorSampleManager;


	SampleManager* sManager = new SampleManager(sampleStore);
	AddItem(sManager);

	/* �o��S��sond binding set���ѭ��q�j�p�A���w�]�o�խ��q�j�p�O0.8 */
	sManager->GetVolume()->SetValue(0.8);

	mirrorSampleVolume->SetValue(1.0);
	sManager->AddAdjustment(AdjustableProperty::Volume, mirrorSampleVolume);
	mirrorSampleVolume->AddOnValueChanged(sManager, bind(&SampleManager::InvalidateState, sManager), "SampleManager::InvalidateState");
	// TODO: �[�Wadjistment 

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
	// TODO: �[�Wadjistment

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

Bindable<double>* AudioManager::GetMirrorSampleVolume()
{
	return mirrorSampleVolume;
}
