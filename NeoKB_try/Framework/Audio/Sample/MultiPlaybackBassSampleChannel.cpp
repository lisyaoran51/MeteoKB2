#include "MultiPlaybackBassSampleChannel.h"

#include "BassSample.h"
#include <math.h>



using namespace Framework::Audio::Samples;



MultiPlaybackBassSampleChannel::MultiPlaybackBassSampleChannel(Sample * s, int pAmount, OverrideType oType): MultiPlaybackSampleChannel(s, pAmount), SampleChannel(s)
{
	overrideType = oType;
}

int MultiPlaybackBassSampleChannel::Play()
{
	return Play(1.0);
}

int MultiPlaybackBassSampleChannel::Play(double v)
{

	if (!GetIsLoaded())
		return -1;

	/* 先將目前正在fadeout的音都暫停fadeout，再播放新的音 */
	StopFadeOut();

	unique_lock<mutex> uLock(pendingActionMutex);

	int channelId = getChannelToPlay();

	LOG(LogLevel::Debug) << "MultiPlaybackBassSampleChannel::Play() : get channel [" << channelId << "] to play.";

	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Debug) << "MultiPlaybackBassSampleChannel::Play() : set channel with volume [" << v * volumeCalculated->GetValue() << "].";

		BASS_ChannelSetAttribute(channelId, BASS_ATTRIB_PAN, 0);
		BASS_ChannelSetAttribute(channelId, BASS_ATTRIB_VOL, v * volumeCalculated->GetValue());

		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::SetChannel");

	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Debug) << "MultiPlaybackBassSampleChannel::Play() : play channel.";

		BASS_ChannelPlay(channelId, true);

		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::Play");

	return 0;
}

int MultiPlaybackBassSampleChannel::Stop()
{
	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		for (int i = 0; i < playbackAmount; i++) {
			if (BASS_ChannelIsActive(channelIds[i]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelPause(channelIds[i]);
			}
		}
		
		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::Stop");

	return 0;
}

int MultiPlaybackBassSampleChannel::FadeOut()
{

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		for (int i = 0; i < playbackAmount; i++) {
			if (BASS_ChannelIsActive(channelIds[i]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelIds[i], BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * 1000));
			}
		}

		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::FadeOut");

	return 0;
}

int MultiPlaybackBassSampleChannel::StopFadeOut()
{
	unique_lock<mutex> uLock(pendingActionMutex);

	pendingActions.Add(this, [=]() {

		for (int i = 0; i < playbackAmount; i++) {
			if (BASS_ChannelIsSliding(channelIds[i], BASS_ATTRIB_VOL)) {
				float v = 0; 
				if (!BASS_ChannelGetAttribute(channelIds[i], BASS_ATTRIB_VOL, &v)) {
					LOG(LogLevel::Error) << "MultiPlaybackBassSampleChannel::StopFadeOut() : get [" << i << "] channel attribute failed. error code: [" << BASS_ErrorGetCode() << "].";
				}
				BASS_ChannelSlideAttribute(channelIds[i], BASS_ATTRIB_VOL, v, 0);
			}
		}
		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::StopFadeOut");


	return 0;
}

bool MultiPlaybackBassSampleChannel::GetIsPlaying()
{
	bool iPlaying = false;

	for (int i = 0; i < playbackAmount; i++) {
		if (BASS_ChannelIsActive(channelIds[i]) == BASS_ACTIVE_PLAYING && !BASS_ChannelIsSliding(channelIds[i], BASS_ATTRIB_VOL)) {
			iPlaying = true;
		}
	}

	return iPlaying;
}

bool MultiPlaybackBassSampleChannel::GetIsLoaded()
{
	return sample->GetIsLoaded();
}

int MultiPlaybackBassSampleChannel::OnStateChange()
{
	AdjustableAudioComponent::OnStateChange();

	for(int i = 0; i < playbackAmount; i++){
		if (!BASS_ChannelIsSliding(channelIds[i], BASS_ATTRIB_VOL)) {
			float v = 0;
			if (!BASS_ChannelGetAttribute(channelIds[i], BASS_ATTRIB_VOL, &v)) {
				LOG(LogLevel::Error) << "MultiPlaybackBassSampleChannel::StopFadeOut() : get [" << i << "] channel attribute failed. error code: [" << BASS_ErrorGetCode() << "].";
			}
			BASS_ChannelSetAttribute(channelIds[i], BASS_ATTRIB_VOL, v * volumeCalculated->GetValue());
		}
	}
}

int MultiPlaybackBassSampleChannel::createSampleChannel()
{
	int channelId = dynamic_cast<BassSample*>(sample)->CreateChannel();

	LOG(LogLevel::Depricated) << "MultiPlaybackBassSampleChannel::createSampleChannel() : get channel id [" << channelId << "].";

	return channelId;
}

int MultiPlaybackBassSampleChannel::getChannelToPlay()
{
	LOG(LogLevel::Debug) << "MultiPlaybackBassSampleChannel::getChannelToPlay() : getting available channel of sample.";

	int channelToPlay = -1;
	for (int i = 0; i < playbackAmount; i++) {
		if (BASS_ChannelIsActive(channelIds[i]) != BASS_ACTIVE_PLAYING) {

			channelToPlay = channelIds[i];
			break;

		}
		else {
			LOG(LogLevel::Debug) << "MultiPlaybackBassSampleChannel::getChannelToPlay() : [" << i << "] channel status is [" << BASS_ChannelIsActive(channelIds[i]) << "].";
		}
	}

	// TODO: 這個是minVolume overrideType的抓法，有需要的話要再寫一個longest overrideType的抓法
	if (channelToPlay = -1) {
		float minVol = 1;
		int minVolChannel = -1;

		for (int i = 0; i < playbackAmount; i++) {
			float v = 0;
			if (!BASS_ChannelGetAttribute(channelIds[i], BASS_ATTRIB_VOL, &v)) {
				LOG(LogLevel::Error) << "MultiPlaybackBassSampleChannel::StopFadeOut() : get [" << i << "] channel attribute failed. error code: [" << BASS_ErrorGetCode() << "].";
			}

			double currentTime = BASS_ChannelBytes2Seconds(channelIds[i], BASS_ChannelGetPosition(channelIds[i], BASS_POS_BYTE));

			/* 音量衰減公式: 音量 = 原始音量 * EXP(-時間) */
			float tempVol = v * exp(-currentTime);

			if (tempVol < minVol) {
				minVol = tempVol;
				minVolChannel = channelIds[i];
			}

		}

		return minVolChannel;
	}

	return channelToPlay;
}
