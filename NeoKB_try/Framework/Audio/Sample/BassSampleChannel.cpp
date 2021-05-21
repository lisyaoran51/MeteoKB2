#include "BassSampleChannel.h"

#include "BassSample.h"

using namespace Framework::Audio::Samples;


BassSampleChannel::BassSampleChannel(Sample * s): SampleChannel(s)
{

}

int BassSampleChannel::Play()
{

	LOG(LogLevel::Debug) << "BassSampleChannel::Play() : add play action.";

	//if (!GetIsPlaying()) {

		unique_lock<mutex> uLock(pendingActionMutex);
		pendingActions.Add(this, [=]() {

			LOG(LogLevel::Debug) << "BassSampleChannel::Play() : create channel.";

			if (!GetIsLoaded()) {
				channelID = -1;
				return 0;
			}

			if (channelID != -1)
				return 0;

			channelID = dynamic_cast<BassSample*>(sample)->CreateChannel();
			BASS_ChannelSetAttribute(channelID, BASS_ATTRIB_PAN, 0);
			BASS_ChannelSetAttribute(channelID, BASS_ATTRIB_VOL, volumeCalculated->GetValue());
			/*
			HFX fxHandle = BASS_ChannelSetFX(channelID, BASS_FX_DX8_REVERB, 0);

			BASS_DX8_REVERB* reverbParameter = new BASS_DX8_REVERB();
			reverbParameter->fInGain = 0;
			reverbParameter->fReverbMix = 0;
			reverbParameter->fReverbTime = 1000;
			reverbParameter->fHighFreqRTRatio = 0.001;
			if (!BASS_FXSetParameters(fxHandle, reverbParameter)) {
				LOG(LogLevel::Error) << "Lambda_BassSampleChannel::CreateChannel() : set reverb error [" << BASS_ErrorGetCode() << "]";
				throw runtime_error("Lambda_BassSampleChannel::CreateChannel() : failed to set reverb");
			}
			*/

			return 0;
		}, "Lambda_BassSampleChannel::CreateChannel");

		pendingActions.Add(this, [=]() {

			LOG(LogLevel::Debug) << "BassSampleChannel::Play() : play channel.";

			BASS_ChannelPlay(channelID, false);

			return 0;
		}, "Lambda_BassSampleChannel::Play");

	//}
	//else {
	//	pendingActions.Add(this, [=]() {
	//
	//		LOG(LogLevel::Debug) << "BassSampleChannel::Play() : play channel.";
	//
	//		BASS_ChannelSetAttribute(channelID, BASS_ATTRIB_VOL, volumeCalculated->GetValue());
	//		BASS_ChannelSetPosition(channelID, 0, BASS_POS_BYTE);
	//
	//		return 0;
	//	}, "Lambda_BassSampleChannel::Replay");
	//}


	return 0;
}

int BassSampleChannel::Play(double v)
{
	volume->SetValue(v);

	return Play();
}

int BassSampleChannel::Stop()
{
	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		delete sample;
		sample = nullptr;
		BASS_ChannelStop(channelID);
		return 0;
	}, "Lambda_BassSampleChannel::Stop");

	return 0;
}

int BassSampleChannel::FadeOut()
{
	BASS_ChannelSlideAttribute(channelID, BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * 1000));

	return 0;
}

int BassSampleChannel::StopFadeOut()
{
	float v = 0;
	if (!BASS_ChannelGetAttribute(channelID, BASS_ATTRIB_VOL, &v)) {
		LOG(LogLevel::Error) << "BassSampleChannel::StopFadeOut() : get channel attribute failed. error code: [" << BASS_ErrorGetCode() << "].";
	}
	BASS_ChannelSlideAttribute(channelID, BASS_ATTRIB_VOL, v, 0);

	return 0;
}

bool BassSampleChannel::GetIsPlaying()
{
	return channelID != -1 && BASS_ChannelIsActive(channelID) == BASS_ACTIVE_PLAYING && !BASS_ChannelIsSliding(channelID, BASS_ATTRIB_VOL);
}

bool BassSampleChannel::GetIsLoaded()
{
	return sample->GetIsLoaded();
}

int BassSampleChannel::OnStateChange()
{
	AdjustableAudioComponent::OnStateChange();

	if (channelID != 0) {

		if(!BASS_ChannelIsSliding(channelID, BASS_ATTRIB_VOL))
			BASS_ChannelSetAttribute(channelID, BASS_ATTRIB_VOL, initialVolume * volumeCalculated->GetValue());

	}

	return 0;
}
