#include "MultiPlaybackBassSampleChannel.h"

#include "BassSample.h"
#include <math.h>



using namespace Framework::Audio::Samples;



MultiPlaybackBassSampleChannel::MultiPlaybackBassSampleChannel(Sample * s, int pAmount, int tAmount, OverrideType oType): MultiPlaybackSampleChannel(s, pAmount, tAmount, oType), SampleChannel(s)
{
}

int MultiPlaybackBassSampleChannel::PlayOnTrack(int trackNumber)
{
	if(trackNumber >= trackAmount)
		throw runtime_error("int MultiPlaybackBassSampleChannel::Play() : track number out of range.");


	return PlayOnTrack(trackNumber, 1.0);
}

int MultiPlaybackBassSampleChannel::PlayOnTrack(int trackNumber, double v)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int MultiPlaybackBassSampleChannel::Play() : track number out of range.");


	if (!GetIsLoaded())
		return -1;

	/* 先將目前正在fadeout的音都暫停fadeout，再播放新的音 */
	StopFadeOut(trackNumber);

	//unique_lock<mutex> uLock(pendingActionMutex);

	int channelId = getChannelToPlay(trackNumber);

	LOG(LogLevel::Depricated) << "MultiPlaybackBassSampleChannel::Play() : get channel [" << channelId << "] to play.";

	BASS_ChannelSetAttribute(channelId, BASS_ATTRIB_PAN, 0);
	BASS_ChannelSetAttribute(channelId, BASS_ATTRIB_VOL, v * volumeCalculated->GetValue());
	BASS_ChannelPlay(channelId, true);

	/*
	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Depricated) << "MultiPlaybackBassSampleChannel::Play() : set channel with volume [" << v * volumeCalculated->GetValue() * 2.0 << "].";

		BASS_ChannelSetAttribute(channelId, BASS_ATTRIB_PAN, 0);
		BASS_ChannelSetAttribute(channelId, BASS_ATTRIB_VOL, v * volumeCalculated->GetValue());

		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::SetChannel");

	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Depricated) << "MultiPlaybackBassSampleChannel::Play() : play channel.";

		BASS_ChannelPlay(channelId, true);

		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::Play");
	*/

	return 0;
}

int MultiPlaybackBassSampleChannel::Stop(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int MultiPlaybackBassSampleChannel::Stop() : track number out of range.");

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		for (int i = 0; i < playbackAmount; i++) {
			if (BASS_ChannelIsActive(channelIds[trackNumber][i]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelPause(channelIds[trackNumber][i]);
			}
		}
		
		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::Stop");

	return 0;
}

int MultiPlaybackBassSampleChannel::FadeOut()
{
	for (int i = 0; i < trackAmount; i++)
		FadeOut(i);

	return 0;
}

int MultiPlaybackBassSampleChannel::FadeOut(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int MultiPlaybackBassSampleChannel::FadeOut() : track number out of range.");

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		/*
		 * 這邊程式寫錯了，直接拿channel的音量來fadeout的話，之後每次調整音量他都會再被縮小音量一次，在on state change那邊會出錯
		 * 不過這個class應該部會在用了，所以懶得改了
		 */

		for (int i = 0; i < playbackAmount; i++) {
			if (BASS_ChannelIsActive(channelIds[trackNumber][i]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * 1000));
			}
		}

		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::FadeOut");

	return 0;
}

int MultiPlaybackBassSampleChannel::StopFadeOut()
{
	for (int i = 0; i < trackAmount; i++)
		StopFadeOut(i);
	return 0;
}

int MultiPlaybackBassSampleChannel::StopFadeOut(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int MultiPlaybackBassSampleChannel::StopFadeOut() : track number out of range.");
	
	for (int i = 0; i < playbackAmount; i++) {
		if (BASS_ChannelIsSliding(channelIds[trackNumber][i], BASS_ATTRIB_VOL)) {
			float v = 0;
			if (!BASS_ChannelGetAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, &v)) {
				LOG(LogLevel::Error) << "MultiPlaybackBassSampleChannel::StopFadeOut() : get [" << i << "] channel attribute failed. error code: [" << BASS_ErrorGetCode() << "].";
			}
			BASS_ChannelSlideAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, v, 0);
		}
	}
	/*
	unique_lock<mutex> uLock(pendingActionMutex);

	pendingActions.Add(this, [=]() {

		for (int i = 0; i < playbackAmount; i++) {
			if (BASS_ChannelIsSliding(channelIds[trackNumber][i], BASS_ATTRIB_VOL)) {
				float v = 0; 
				if (!BASS_ChannelGetAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, &v)) {
					LOG(LogLevel::Error) << "MultiPlaybackBassSampleChannel::StopFadeOut() : get [" << i << "] channel attribute failed. error code: [" << BASS_ErrorGetCode() << "].";
				}
				BASS_ChannelSlideAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, v, 0);
			}
		}
		return 0;
	}, "Lambda_MultiPlaybackBassSampleChannel::StopFadeOut");
	*/

	return 0;
}

bool MultiPlaybackBassSampleChannel::GetIsPlaying()
{
	for (int i = 0; i < trackAmount; i++)
		if (GetIsPlaying(i))
			return true;

	return false;
}

bool MultiPlaybackBassSampleChannel::GetIsPlaying(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int MultiPlaybackBassSampleChannel::GetIsPlaying() : track number out of range.");

	bool iPlaying = false;

	for (int i = 0; i < playbackAmount; i++) {
		if (BASS_ChannelIsActive(channelIds[trackNumber][i]) == BASS_ACTIVE_PLAYING && !BASS_ChannelIsSliding(channelIds[trackNumber][i], BASS_ATTRIB_VOL)) {
			iPlaying = true;
		}
	}

	return iPlaying;
}

bool MultiPlaybackBassSampleChannel::GetIsLoaded()
{

	return sample->GetIsLoaded();
}

int MultiPlaybackBassSampleChannel::OnStateChange(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int MultiPlaybackBassSampleChannel::OnStateChange() : track number out of range.");

	AdjustableAudioComponent::OnStateChange();

	for(int i = 0; i < playbackAmount; i++){
		if (!BASS_ChannelIsSliding(channelIds[trackNumber][i], BASS_ATTRIB_VOL)) {
			/*
			 * 這邊程式寫錯了，直接拿channel的音量來與volumeCalculated相乘的話，之後每次調整音量他都會再被縮小音量一次，音量大小會出錯
			 * 不過這個class應該部會在用了，所以懶得改了
			 */
			float v = 0;
			if (!BASS_ChannelGetAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, &v)) {
				LOG(LogLevel::Error) << "MultiPlaybackBassSampleChannel::StopFadeOut() : get [" << i << "] channel attribute failed. error code: [" << BASS_ErrorGetCode() << "].";
			}
			BASS_ChannelSetAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, v * volumeCalculated->GetValue());
		}
	}
}

int MultiPlaybackBassSampleChannel::createSampleChannel()
{
	int channelId = dynamic_cast<BassSample*>(sample)->CreateChannel();

	LOG(LogLevel::Depricated) << "MultiPlaybackBassSampleChannel::createSampleChannel() : get channel id [" << channelId << "].";

	return channelId;
}

int MultiPlaybackBassSampleChannel::getChannelToPlay(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int MultiPlaybackBassSampleChannel::getChannelToPlay() : track number out of range.");

	LOG(LogLevel::Depricated) << "MultiPlaybackBassSampleChannel::getChannelToPlay() : getting available channel of sample.";

	int channelToPlay = -1;
	for (int i = 0; i < playbackAmount; i++) {
		if (BASS_ChannelIsActive(channelIds[trackNumber][i]) != BASS_ACTIVE_PLAYING) {

			LOG(LogLevel::Depricated) << "MultiPlaybackBassSampleChannel::getChannelToPlay() : [" << channelIds[trackNumber][i] << "] channel status is [" << BASS_ChannelIsActive(channelIds[trackNumber][i]) << "].";
			channelToPlay = channelIds[trackNumber][i];
			break;

		}
		else{
			LOG(LogLevel::Depricated) << "MultiPlaybackBassSampleChannel::getChannelToPlay() : [" << i << "] channel status is [" << BASS_ChannelIsActive(channelIds[trackNumber][i]) << "].";
		}
	}

	// TODO: 這個是minVolume overrideType的抓法，有需要的話要再寫一個longest overrideType的抓法
	if (channelToPlay == -1) {
		float minVol = 1;
		int minVolChannel = -1;

		for (int i = 0; i < playbackAmount; i++) {
			float v = 0;
			if (!BASS_ChannelGetAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, &v)) {
				LOG(LogLevel::Error) << "MultiPlaybackBassSampleChannel::getChannelToPlay() : get [" << i << "] channel attribute failed. error code: [" << BASS_ErrorGetCode() << "].";
			}

			double currentTime = BASS_ChannelBytes2Seconds(channelIds[trackNumber][i], BASS_ChannelGetPosition(channelIds[trackNumber][i], BASS_POS_BYTE));

			/* 音量衰減公式: 音量 = 原始音量 * EXP(-時間) */
			float tempVol = v * exp(-currentTime);

			if (tempVol < minVol) {
				minVol = tempVol;
				minVolChannel = channelIds[trackNumber][i];
			}

		}

		return minVolChannel;
	}

	return channelToPlay;
}
