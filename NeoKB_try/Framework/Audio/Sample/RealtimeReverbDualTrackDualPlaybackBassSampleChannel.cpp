#include "RealtimeReverbDualTrackDualPlaybackBassSampleChannel.h"

#include "BassSample.h"
#include <cmath>


using namespace Framework::Audio::Samples;


RealtimeReverbDualTrackDualPlaybackBassSampleChannel::RealtimeReverbDualTrackDualPlaybackBassSampleChannel(Sample * s): DualTrackDualPlaybackBassSampleChannel(s)
{
	for(int i = 0; i < 5; i++)
		reverbChannelID[i] = dynamic_cast<BassSample*>(sample)->CreateChannel();

	reverbVolumes[0] = 0.05;
	reverbVolumes[0] = 0.06;
	reverbVolumes[0] = 0.07;
	reverbVolumes[0] = 0.08;
	reverbVolumes[0] = 0.09;
}

RealtimeReverbDualTrackDualPlaybackBassSampleChannel::~RealtimeReverbDualTrackDualPlaybackBassSampleChannel()
{
	for (int i = 0; i < 5; i++)
		BASS_ChannelStop(reverbChannelID[i]);
}

int RealtimeReverbDualTrackDualPlaybackBassSampleChannel::Play()
{
	LOG(LogLevel::Depricated) << "RealtimeReverbDualTrackDualPlaybackBassSampleChannel::Play() : add play action with volume calculated [" << volumeCalculated->GetValue() << "].";

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

		LOG(LogLevel::Depricated) << "DualTrackDualPlaybackBassSampleChannel::Play() : last voume [" << tempVolume << "], new volume [" << volume->GetValue() << "], pan [" << pan << ".";


		if (tempPlayingPlayback == 0)
			newPlayback = 1;
		else
			newPlayback = 0;

		BASS_ChannelPause(channelID[newPlayback]);
		BASS_ChannelSetAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume);// / 4.f);
		BASS_ChannelSetAttribute(channelID[newPlayback], BASS_ATTRIB_PAN, pan);
		BASS_ChannelSetPosition(channelID[newPlayback], 0, BASS_POS_BYTE);
		/* 檢查是否在fadeout，是的話把fadeout停掉 */
		if (BASS_ChannelIsSliding(channelID[newPlayback], BASS_ATTRIB_VOL) == TRUE)
			BASS_ChannelSlideAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume, (DWORD)(0));// / 4.f, (DWORD)(0));

		BASS_ChannelPlay(channelID[newPlayback], false);

		if (tempVolume <= volumeCalculated->GetValue()) {
			if (BASS_ChannelIsActive(channelID[tempPlayingPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelID[tempPlayingPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(dualSwitchFadeoutTime * 1000));
			}
			tempPlayingPlayback = newPlayback;
		}
		else {
			LOG(LogLevel::Depricated) << "DualTrackDualPlaybackBassSampleChannel::Play() : last voume [" << tempVolume << "], louder than new volume [" << volume->GetValue() << "].";
			if (BASS_ChannelIsActive(channelID[newPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(dualSwitchFadeoutTime * 1000));
			}
			volume->SetValue(lastVolume);
		}

		/* 將所有reverb在predelay之前fadeout */
		for (int i = 0; i < 5; i++) {
			if (BASS_ChannelIsActive(reverbChannelID[i]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(reverbChannelID[i], BASS_ATTRIB_VOL, 0, (DWORD)(predelay * 1000));
			}
			BASS_ChannelSlideAttribute(reverbChannelID[i], BASS_ATTRIB_PAN, pan, (DWORD)(predelay * 1000));
		}
		


		return 0;
	}, "Lambda_DualTrackDualPlaybackBassSampleChannel::Play");
	uLock.unlock();

	/* 分別建立所有reverb */
	unique_lock<mutex> uLock2(timedActionMutex);
	for (int i = 0; i < 5; i++) {

		timedActions.clear();

		timedActions.push_back(pair<float, function<int()>>(predelay + delays[i], [=]() {

			BASS_ChannelPause(reverbChannelID[i]);
			BASS_ChannelSetAttribute(reverbChannelID[i], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume * reverbVolumes[i]);// / 4.f);
			BASS_ChannelSetPosition(reverbChannelID[i], 0, BASS_POS_BYTE);
			/* 檢查是否在fadeout，是的話把fadeout停掉 */
			if (BASS_ChannelIsSliding(reverbChannelID[i], BASS_ATTRIB_VOL) == TRUE)
				BASS_ChannelSlideAttribute(reverbChannelID[i], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume * reverbVolumes[i], (DWORD)(0));// / 4.f, (DWORD)(0));

			BASS_ChannelPlay(reverbChannelID[i], false);

			return 0;
		}));
	}
	


	return 0;
}

int RealtimeReverbDualTrackDualPlaybackBassSampleChannel::FadeOut()
{
	LOG(LogLevel::Depricated) << "RealtimeReverbDualTrackDualPlaybackBassSampleChannel::FadeOut() : fadeout reverb channel.";

	unique_lock<mutex> uLock(timedActionMutex);
	timedActions.clear();
	uLock.unlock();

	unique_lock<mutex> uLock2(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		for (int i = 0; i < 5; i++) {
			if (BASS_ChannelIsActive(reverbChannelID[i]) == BASS_ACTIVE_PLAYING)
				if (!BASS_ChannelSlideAttribute(reverbChannelID[i], BASS_ATTRIB_VOL, 0, (DWORD)(reverbFadeoutTime * 1000))) {
					LOG(LogLevel::Debug) << "RealtimeReverbDualTrackDualPlaybackBassSampleChannel::FadeOut() : fail to slide." << BASS_ErrorGetCode();
				}
		}
		return 0;

	}, "Lambda_RealtimeReverbDualTrackDualPlaybackBassSampleChannel::FadeOut");
	uLock2.unlock();

	return DualPlaybackBassSampleChannel::FadeOut();
}
