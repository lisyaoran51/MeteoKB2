#include "DualTrackDualPlaybackBassSampleChannel.h"

#include <cmath>



using namespace Framework::Audio::Samples;


DualTrackDualPlaybackBassSampleChannel::DualTrackDualPlaybackBassSampleChannel(Sample * s) : DualPlaybackBassSampleChannel(s)
{
	gradientTimbreStartVolume = 0.1f;
	gradientTimbreEndVolume = 0.7f;
}

int DualTrackDualPlaybackBassSampleChannel::Update()
{

	/* 把已經Fadeout的音關掉 */
	if (isPlaying) {
		for (int i = 0; i < 2; i++) {
			if (BASS_ChannelIsActive(channelID[i]) == BASS_ACTIVE_PLAYING) {
				float tempVolume = 0;
				BASS_ChannelGetAttribute(channelID[i], BASS_ATTRIB_VOL, &tempVolume);
				if (tempVolume == 0) {

					LOG(LogLevel::Depricated) << "DualTrackDualPlaybackBassSampleChannel::Update() : turn off channel [" << channelID[i] << "].";
					BASS_ChannelPause(channelID[i]);
				}
			}
		}
	}
	return 0;
}

int DualTrackDualPlaybackBassSampleChannel::Play()
{
	LOG(LogLevel::Depricated) << "DualTrackDualPlaybackBassSampleChannel::Play() : add play action with volume calculated [" << volumeCalculated->GetValue() << "].";

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {
		int newPlayback = 0;

		float lastPlayVolume = lastChannelVolume;
		if (BASS_ChannelIsActive(channelID[tempPlayingPlayback]) == BASS_ACTIVE_PLAYING) {

			// 音量衰減公式 音量=e(-at)，a為常數，t為時間

			double tempPlaybackCurrentTime = BASS_ChannelBytes2Seconds(
				channelID[tempPlayingPlayback],
				BASS_ChannelGetPosition(channelID[tempPlayingPlayback], BASS_POS_BYTE));

			// TODO: 衰退太快，實際聲音沒有衰退那麼快。不過如果衰退太慢會有聲音斷掉的問題
			lastPlayVolume = lastPlayVolume * exp(-tempPlaybackCurrentTime * 1.3);	//試試看衰退時間縮短，1.3是一個剛好的數字，
																					// 原本音色保留要稍微強一點，即使新音色可能聲音稍微大衣歇歇，但是還是要先以原本音色為主
																					// 如果新音色真的大比較多的時候，再把原本音色蓋掉
			LOG(LogLevel::Depricated) << "DualTrackDualPlaybackBassSampleChannel::Play() : last voume [" << lastPlayVolume << "], new volume [" << volume->GetValue() << "], calculated volume [" << volumeCalculated->GetValue() << "]";

		}
		else
			lastPlayVolume = 0;

		// 音質調整公式 pan = (volume - 0.3) / (0.7 - 0.3)，最大為1，最小為0

		float pan = (volume->GetValue() - gradientTimbreStartVolume) / (gradientTimbreEndVolume - gradientTimbreStartVolume);
		if (pan > 1)pan = 1;
		if (pan < 0)pan = 0;

		LOG(LogLevel::Depricated) << "DualTrackDualPlaybackBassSampleChannel::Play() : last voume [" << lastPlayVolume << "], new volume [" << volumeCalculated->GetValue() << "], pan [" << pan << ".";


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

		if (lastPlayVolume <= volumeCalculated->GetValue()) {
			if (BASS_ChannelIsActive(channelID[tempPlayingPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelID[tempPlayingPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(dualSwitchFadeoutTime * 1000));
			}
			tempPlayingPlayback = newPlayback;
		}
		else {
			LOG(LogLevel::Depricated) << "DualTrackDualPlaybackBassSampleChannel::Play() : last voume [" << lastPlayVolume << "], louder than new volume [" << volume->GetValue() << "].";
			if (BASS_ChannelIsActive(channelID[newPlayback]) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelSlideAttribute(channelID[newPlayback], BASS_ATTRIB_VOL, 0, (DWORD)(dualSwitchFadeoutTime * 1000));
			}
			volume->SetValue(lastVolume);
		}


		return 0;
	}, "Lambda_DualTrackDualPlaybackBassSampleChannel::Play");

	return 0;
}

int DualTrackDualPlaybackBassSampleChannel::SetTimbreRange(float start, float end)
{
	LOG(LogLevel::Depricated) << "DualTrackDualPlaybackBassSampleChannel::SetTimbreRange() : start [" << start << "], end [" << end << "].";

	gradientTimbreStartVolume = start;
	gradientTimbreEndVolume = end;

	return 0;
}
