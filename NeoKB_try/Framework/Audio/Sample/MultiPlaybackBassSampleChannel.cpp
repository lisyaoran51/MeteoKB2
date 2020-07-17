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

	/* ���N�ثe���bfadeout�������Ȱ�fadeout�A�A����s���� */
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
		 * �o��{���g���F�A������channel�����q��fadeout���ܡA����C���վ㭵�q�L���|�A�Q�Y�p���q�@���A�bon state change����|�X��
		 * ���L�o��class���ӳ��|�b�ΤF�A�ҥH�i�o��F
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
			 * �o��{���g���F�A������channel�����q�ӻPvolumeCalculated�ۭ����ܡA����C���վ㭵�q�L���|�A�Q�Y�p���q�@���A���q�j�p�|�X��
			 * ���L�o��class���ӳ��|�b�ΤF�A�ҥH�i�o��F
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

	// TODO: �o�ӬOminVolume overrideType����k�A���ݭn���ܭn�A�g�@��longest overrideType����k
	if (channelToPlay == -1) {
		float minVol = 1;
		int minVolChannel = -1;

		for (int i = 0; i < playbackAmount; i++) {
			float v = 0;
			if (!BASS_ChannelGetAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, &v)) {
				LOG(LogLevel::Error) << "MultiPlaybackBassSampleChannel::getChannelToPlay() : get [" << i << "] channel attribute failed. error code: [" << BASS_ErrorGetCode() << "].";
			}

			double currentTime = BASS_ChannelBytes2Seconds(channelIds[trackNumber][i], BASS_ChannelGetPosition(channelIds[trackNumber][i], BASS_POS_BYTE));

			/* ���q�I���: ���q = ��l���q * EXP(-�ɶ�) */
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
