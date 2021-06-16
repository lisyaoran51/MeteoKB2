#include "ReverbDualTrackDualPlaybackBassSampleChannel.h"

#include "BassSample.h"
#include "BassSampleChannelGenerator.h"
#include <cmath>

using namespace Framework::Audio::Samples;



ReverbDualTrackDualPlaybackBassSampleChannel::ReverbDualTrackDualPlaybackBassSampleChannel(Sample * s, Sample * rSample) : DualTrackDualPlaybackBassSampleChannel(s)
{
	reverbSample = rSample; 
	LOG(LogLevel::Fine) << "ReverbDualTrackDualPlaybackBassSampleChannel::ReverbDualTrackDualPlaybackBassSampleChannel() : get channels from sample." << rSample;

	reverbChannelID[0] = dynamic_cast<BassSample*>(rSample)->CreateChannel();
	reverbChannelID[1] = dynamic_cast<BassSample*>(rSample)->CreateChannel();
}

ReverbDualTrackDualPlaybackBassSampleChannel::~ReverbDualTrackDualPlaybackBassSampleChannel()
{
	LOG(LogLevel::Depricated) << "ReverbDualTrackDualPlaybackBassSampleChannel::~ReverbDualTrackDualPlaybackBassSampleChannel() : delete channels.";
	BASS_ChannelStop(reverbChannelID[0]);
	BASS_ChannelStop(reverbChannelID[1]);

	BassSampleChannelGenerator::MoveSampleToDeleteCache(reverbSample);

	//delete sample;
	reverbSample = nullptr;
}

int ReverbDualTrackDualPlaybackBassSampleChannel::Play()
{
	LOG(LogLevel::Debug) << "ReverbDualTrackDualPlaybackBassSampleChannel::Play() : add play action with volume calculated [" << volumeCalculated->GetValue() << "].";

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {
		int newPlayback = 0;

		float lastChannelVolume;
		BASS_ChannelGetAttribute(channelID[tempPlayingPlayback], BASS_ATTRIB_VOL, &lastChannelVolume);

		// 音量衰減公式 音量=e(-at)，a為常數，t為時間

		double tempPlaybackCurrentTime = BASS_ChannelBytes2Seconds(
			channelID[tempPlayingPlayback],
			BASS_ChannelGetPosition(channelID[tempPlayingPlayback], BASS_POS_BYTE));

		double tempVolume = lastChannelVolume * exp(-tempPlaybackCurrentTime);

		if (BASS_ChannelIsActive(channelID[tempPlayingPlayback]) != BASS_ACTIVE_PLAYING) {
			tempVolume = 0;
		}

		// 音質調整公式 pan = -1 + ((volume - 0.3) / (0.7 - 0.3) * 2，最大為1，最小為-1

		float pan = -1.f + (volume->GetValue() - gradientTimbreStartVolume) / (gradientTimbreEndVolume - gradientTimbreStartVolume) * 2.f;
		if (pan > 1)pan = 1;
		if (pan < -1)pan = -1;

		LOG(LogLevel::Depricated) << "ReverbDualTrackDualPlaybackBassSampleChannel::Play() : last voume [" << tempVolume << "], new volume [" << volume->GetValue() << "], pan [" << pan << ".";


		if (tempPlayingPlayback == 0)
			newPlayback = 1;
		else
			newPlayback = 0;

		// 原sample
		BASS_ChannelPause(channelID[newPlayback]);
		BASS_ChannelSetAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume);// / 4.f);
		BASS_ChannelSetAttribute(channelID[newPlayback], BASS_ATTRIB_PAN, pan);
		BASS_ChannelSetPosition(channelID[newPlayback], 0, BASS_POS_BYTE);
		/* 檢查是否在fadeout，是的話把fadeout停掉 */
		if (BASS_ChannelIsSliding(channelID[newPlayback], BASS_ATTRIB_VOL) == TRUE)
			BASS_ChannelSlideAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume, (DWORD)(0));// / 4.f, (DWORD)(0));

		// reverb sample
		BASS_ChannelPause(reverbChannelID[newPlayback]);
		BASS_ChannelSetAttribute(reverbChannelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume);// / 4.f);
		BASS_ChannelSetPosition(reverbChannelID[newPlayback], 0, BASS_POS_BYTE);
		/* 檢查是否在fadeout，是的話把fadeout停掉 */
		if (BASS_ChannelIsSliding(reverbChannelID[newPlayback], BASS_ATTRIB_VOL) == TRUE)
			BASS_ChannelSlideAttribute(reverbChannelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume, (DWORD)(0));// / 4.f, (DWORD)(0));


		BASS_ChannelPlay(channelID[newPlayback], false);
		BASS_ChannelPlay(reverbChannelID[newPlayback], false);

		if (tempVolume <= volumeCalculated->GetValue()) {
			if (BASS_ChannelIsActive(channelID[tempPlayingPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelID[tempPlayingPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(dualSwitchFadeoutTime * 1000));
			}

			if (BASS_ChannelIsActive(reverbChannelID[tempPlayingPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(reverbChannelID[tempPlayingPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(dualSwitchFadeoutTime * 1000));
			}
			tempPlayingPlayback = newPlayback;
		}
		else {
			LOG(LogLevel::Depricated) << "ReverbDualTrackDualPlaybackBassSampleChannel::Play() : last voume [" << tempVolume << "], louder than new volume [" << volume->GetValue() << "].";
			if (BASS_ChannelIsActive(channelID[newPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(dualSwitchFadeoutTime * 1000));
			}

			if (BASS_ChannelIsActive(reverbChannelID[newPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(reverbChannelID[newPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(dualSwitchFadeoutTime * 1000));
			}
			volume->SetValue(lastVolume);
		}


		return 0;
	}, "Lambda_ReverbDualTrackDualPlaybackBassSampleChannel::Play");

	return 0;
}

int ReverbDualTrackDualPlaybackBassSampleChannel::FadeOut()
{
	LOG(LogLevel::Debug) << "ReverbDualTrackDualPlaybackBassSampleChannel::FadeOut() : fadeout reverb channel.";


	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {
		

		if (BASS_ChannelIsActive(reverbChannelID[0]) == BASS_ACTIVE_PLAYING)
			if (!BASS_ChannelSlideAttribute(reverbChannelID[0], BASS_ATTRIB_VOL, 0, (DWORD)(reverbFadeoutTime * 1000))) {
				LOG(LogLevel::Debug) << "ReverbDualTrackDualPlaybackBassSampleChannel::FadeOut() : fail to slide." << BASS_ErrorGetCode();
			}

		if (BASS_ChannelIsActive(reverbChannelID[1]) == BASS_ACTIVE_PLAYING)
			if (!BASS_ChannelSlideAttribute(reverbChannelID[1], BASS_ATTRIB_VOL, 0, (DWORD)(reverbFadeoutTime * 1000))) {
				LOG(LogLevel::Debug) << "ReverbDualTrackDualPlaybackBassSampleChannel::FadeOut() : fail to slide." << BASS_ErrorGetCode();
			}
		return 0;

	}, "Lambda_ReverbDualTrackDualPlaybackBassSampleChannel::FadeOut");
	uLock.unlock();

	return DualPlaybackBassSampleChannel::FadeOut();
}
