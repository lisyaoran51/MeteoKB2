#include "DualPlaybackBassSampleChannel.h"

#include "BassSample.h"

using namespace Framework::Audio::Samples;


DualPlaybackBassSampleChannel::DualPlaybackBassSampleChannel(Sample * s) : SampleChannel(s)
{
	channelID[0] = dynamic_cast<BassSample*>(sample)->CreateChannel();
	channelID[1] = dynamic_cast<BassSample*>(sample)->CreateChannel();
}

DualPlaybackBassSampleChannel::~DualPlaybackBassSampleChannel()
{
	BASS_ChannelStop(channelID[0]);
	BASS_ChannelStop(channelID[1]);
}

int DualPlaybackBassSampleChannel::Play()
{
	LOG(LogLevel::Depricated) << "DualPlaybackBassSampleChannel::Play() : add play action.";
	
	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {
		int newPlayback = 0;

		if (tempPlayingPlayback == 0)
			newPlayback = 1;
		else
			newPlayback = 0;

		BASS_ChannelPause(channelID[newPlayback]);
		BASS_ChannelSetAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() / 4.f);
		BASS_ChannelSetPosition(channelID[newPlayback], 0, BASS_POS_BYTE);
		/* 檢查是否在fadeout，是的話把fadeout停掉 */
		if (BASS_ChannelIsSliding(channelID[newPlayback], BASS_ATTRIB_VOL) == TRUE)
			BASS_ChannelSlideAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() / 4.f, (DWORD)(0));

		BASS_ChannelPlay(channelID[newPlayback], false);

		if (BASS_ChannelIsActive(channelID[tempPlayingPlayback]) == BASS_ACTIVE_PLAYING) {
			BASS_ChannelSlideAttribute(channelID[tempPlayingPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime / 4 * 1000));
		}

		tempPlayingPlayback = newPlayback;

		return 0;
	}, "Lambda_DualPlaybackBassSampleChannel::Play");

	return 0;
}

int DualPlaybackBassSampleChannel::Play(double v)
{
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
			BASS_ChannelSlideAttribute(channelID[1], BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * 1000));
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

//int DualPlaybackBassSampleChannel::OnStateChange()
//{
//	return 0;
//}
