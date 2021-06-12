#include "DualPlaybackBassSampleChannel.h"

#include "BassSample.h"
#include <cmath>
#include "BassSampleChannelGenerator.h"

using namespace Framework::Audio::Samples;


DualPlaybackBassSampleChannel::DualPlaybackBassSampleChannel(Sample * s) : SampleChannel(s)
{
	LOG(LogLevel::Depricated) << "DualPlaybackBassSampleChannel::DualPlaybackBassSampleChannel() : create channels." << this;
	volume->SetValue(0);
	channelID[0] = dynamic_cast<BassSample*>(sample)->CreateChannel();
	channelID[1] = dynamic_cast<BassSample*>(sample)->CreateChannel();
}

DualPlaybackBassSampleChannel::~DualPlaybackBassSampleChannel()
{
	LOG(LogLevel::Depricated) << "DualPlaybackBassSampleChannel::~DualPlaybackBassSampleChannel() : delete channels.";
	BASS_ChannelStop(channelID[0]);
	BASS_ChannelStop(channelID[1]);

	BassSampleChannelGenerator::MoveSampleToDeleteCache(sample);

	//delete sample;
	sample = nullptr;
}

int DualPlaybackBassSampleChannel::Play()
{
	LOG(LogLevel::Depricated) << "DualPlaybackBassSampleChannel::Play() : add play action with volume calculated [" << volumeCalculated->GetValue() << "].";
	
	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {
		int newPlayback = 0;

		// 音量衰減公式 音量=e(-at)，a為常數，t為時間


		double tempPlaybackCurrentTime = BASS_ChannelBytes2Seconds(
			channelID[tempPlayingPlayback],
			BASS_ChannelGetPosition(channelID[tempPlayingPlayback], BASS_POS_BYTE));

		double tempVolume = lastVolume * exp(-tempPlaybackCurrentTime);

		if (BASS_ChannelIsActive(channelID[tempPlayingPlayback]) != BASS_ACTIVE_PLAYING) {
			tempVolume = 0;
		}

		
		LOG(LogLevel::Depricated) << "DualPlaybackBassSampleChannel::Play() : last voume [" << tempVolume << "], new volume [" << volume->GetValue() << "].";


		if (tempPlayingPlayback == 0)
			newPlayback = 1;
		else
			newPlayback = 0;

		BASS_ChannelPause(channelID[newPlayback]);
		BASS_ChannelSetAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume);// / 4.f);
		BASS_ChannelSetPosition(channelID[newPlayback], 0, BASS_POS_BYTE);
		/* 檢查是否在fadeout，是的話把fadeout停掉 */
		if (BASS_ChannelIsSliding(channelID[newPlayback], BASS_ATTRIB_VOL) == TRUE)
			BASS_ChannelSlideAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume, (DWORD)(0));// / 4.f, (DWORD)(0));

		BASS_ChannelPlay(channelID[newPlayback], false);

		if (tempVolume <= volume->GetValue()) {
			if (BASS_ChannelIsActive(channelID[tempPlayingPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelID[tempPlayingPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime / 2 * 1000));
			}
			tempPlayingPlayback = newPlayback;
		}
		else {
			LOG(LogLevel::Depricated) << "DualPlaybackBassSampleChannel::Play() : last voume [" << tempVolume << "], louder than new volume [" << volume->GetValue() << "].";
			if (BASS_ChannelIsActive(channelID[newPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * 2 * 1000));
			}
			volume->SetValue(lastVolume);
		}


		return 0;
	}, "Lambda_DualPlaybackBassSampleChannel::Play");

	return 0;
}

int DualPlaybackBassSampleChannel::Play(double v)
{
	lastVolume = volume->GetValue();
	volume->SetValue(v);
	InvalidateState();
	return Play();
}

int DualPlaybackBassSampleChannel::Stop()
{
	return 0;
}

int DualPlaybackBassSampleChannel::FadeOut()
{
	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {
		if (BASS_ChannelIsActive(channelID[0]) == BASS_ACTIVE_PLAYING)
			BASS_ChannelSlideAttribute(channelID[0], BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * 1000));

		if (BASS_ChannelIsActive(channelID[1]) == BASS_ACTIVE_PLAYING)
			BASS_ChannelSlideAttribute(channelID[1], BASS_ATTRIB_VOL, 0, (DWORD)(0.05 * 1000)); //dualSwitchFadeoutTime * 1000));
		return 0;

	}, "Lambda_DualPlaybackBassSampleChannel::FadeOut");
	return 0;
}

int DualPlaybackBassSampleChannel::StopFadeOut()
{
	return 0;
}

bool DualPlaybackBassSampleChannel::GetIsPlaying()
{
	if (BASS_ChannelIsActive(channelID[0]) == BASS_ACTIVE_PLAYING ||
		BASS_ChannelIsActive(channelID[1]) == BASS_ACTIVE_PLAYING)
		return true;

	return false;
}

bool DualPlaybackBassSampleChannel::GetIsLoaded()
{
	return false;
}

int DualPlaybackBassSampleChannel::OnStateChange()
{
	AdjustableAudioComponent::OnStateChange();

	if (channelID[tempPlayingPlayback] != 0) {

		if (!BASS_ChannelIsSliding(channelID[tempPlayingPlayback], BASS_ATTRIB_VOL))
			BASS_ChannelSetAttribute(channelID[tempPlayingPlayback], BASS_ATTRIB_VOL, initialVolume * volumeCalculated->GetValue());

	}

	return 0;
}

//int DualPlaybackBassSampleChannel::OnStateChange()
//{
//	return 0;
//}
