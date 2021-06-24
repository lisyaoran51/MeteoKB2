#include "RealtimeReverbDualTrackDualPlaybackBassSampleChannel.h"

#include "BassSample.h"
#include <cmath>


using namespace Framework::Audio::Samples;


RealtimeReverbDualTrackDualPlaybackBassSampleChannel::RealtimeReverbDualTrackDualPlaybackBassSampleChannel(Sample * s): DualTrackDualPlaybackBassSampleChannel(s)
{
	for(int i = 0; i < 5; i++)
		reverbChannelID[i] = dynamic_cast<BassSample*>(sample)->CreateChannel();

	// 大小2
	reverbVolumes[0] = 0.13;
	reverbVolumes[1] = 0.14;
	reverbVolumes[2] = 0.15;
	reverbVolumes[3] = 0.16;
	reverbVolumes[4] = 0.17;

	// 大小3
	//reverbVolumes[0] = 0.18;
	//reverbVolumes[1] = 0.19;
	//reverbVolumes[2] = 0.20;
	//reverbVolumes[3] = 0.21;
	//reverbVolumes[4] = 0.22;

	predelay = 0.08f;
	reverbFadeoutTime = 1.f;
}

RealtimeReverbDualTrackDualPlaybackBassSampleChannel::~RealtimeReverbDualTrackDualPlaybackBassSampleChannel()
{
	for (int i = 0; i < 5; i++)
		BASS_ChannelStop(reverbChannelID[i]);
}

int RealtimeReverbDualTrackDualPlaybackBassSampleChannel::Update()
{
	/* 把已經Fadeout的音關掉 */
	if (isPlaying) {
		for (int i = 0; i < 2; i++) {
			if (BASS_ChannelIsActive(channelID[i]) == BASS_ACTIVE_PLAYING) {
				float tempVolume = 0;
				BASS_ChannelGetAttribute(channelID[i], BASS_ATTRIB_VOL, &tempVolume);
				if (tempVolume == 0)
					BASS_ChannelPause(channelID[i]);
			}
		}
	}

	/* 把已經Fadeout的reverb關掉 */
	if (BASS_ChannelIsActive(reverbChannelID[0]) == BASS_ACTIVE_PLAYING) {

		float tempReverbVolume = 0;
		BASS_ChannelGetAttribute(reverbChannelID[0], BASS_ATTRIB_VOL, &tempReverbVolume);
		if(tempReverbVolume == 0) {
			LOG(LogLevel::Depricated) << "RealtimeReverbDualTrackDualPlaybackBassSampleChannel::Update() : fadeout low volume.";

			goto PAUSE_REVERB;
		}

		/* 如果時間超過總長的一定比例，就關掉reverb。越用力，reverb舊月九 */
		if ((float)BASS_ChannelGetPosition(reverbChannelID[0], BASS_POS_BYTE) / (float)BASS_ChannelGetLength(reverbChannelID[0], BASS_POS_BYTE) > volume->GetValue()) {
			LOG(LogLevel::Depricated) << "RealtimeReverbDualTrackDualPlaybackBassSampleChannel::Update() : fadeout too long.";
			goto PAUSE_REVERB;
		}
	}

	return SampleChannel::Update();

PAUSE_REVERB:
	for (int i = 0; i < 5; i++)
		BASS_ChannelPause(reverbChannelID[i]);

	return SampleChannel::Update();

}

int RealtimeReverbDualTrackDualPlaybackBassSampleChannel::Play()
{
	LOG(LogLevel::Depricated) << "RealtimeReverbDualTrackDualPlaybackBassSampleChannel::Play() : add play action with volume calculated [" << volumeCalculated->GetValue() << "].";

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {
		int newPlayback = 0;

		float lastPlayVolume = lastVolume;
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
		

		// 音質調整公式 pan = -1 + ((volume - 0.3) / (0.7 - 0.3) * 2，最大為1，最小為-1

		float pan = -1.f + (volume->GetValue() - gradientTimbreStartVolume) / (gradientTimbreEndVolume - gradientTimbreStartVolume) * 2.f;
		if (pan > 1)pan = 1;
		if (pan < -1)pan = -1;

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

			/* 分別建立所有reverb */
			unique_lock<mutex> uLock2(timedActionMutex);
			timedActions.clear();
			for (int i = 0; i < 5; i++) {


				timedActions.push_back(pair<float, function<int()>>(predelay + delays[i], [=]() {

					/* reverb的開頭用fadein近來，財部會有重複琴見音 */
					float reverbFadeinTime = 0.05;

					BASS_ChannelPause(reverbChannelID[i]);
					BASS_ChannelSetAttribute(reverbChannelID[i], BASS_ATTRIB_VOL, 0);// / 4.f);
					BASS_ChannelSetPosition(reverbChannelID[i], 0, BASS_POS_BYTE);
					/* 檢查是否在fadeout，是的話把fadeout停掉 */
					//if (BASS_ChannelIsSliding(reverbChannelID[i], BASS_ATTRIB_VOL) == TRUE)
					//	BASS_ChannelSlideAttribute(reverbChannelID[i], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume * reverbVolumes[i], (DWORD)(0));// / 4.f, (DWORD)(0));
					BASS_ChannelSlideAttribute(reverbChannelID[i], BASS_ATTRIB_VOL, volumeCalculated->GetValue() * initialVolume * reverbVolumes[i], (DWORD)(reverbFadeinTime * 1000));// / 4.f, (DWORD)(0));

					BASS_ChannelPlay(reverbChannelID[i], false);

					return 0;
				}));
			}

		}
		else {
			LOG(LogLevel::Depricated) << "DualTrackDualPlaybackBassSampleChannel::Play() : last voume [" << lastPlayVolume << "], louder than new volume [" << volumeCalculated->GetValue() << "].";
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
