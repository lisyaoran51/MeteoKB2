#include "BassSampleChannel.h"

#include "BassSample.h"

using namespace Framework::Audio::Samples;


BassSampleChannel::BassSampleChannel(Sample * s): SampleChannel(s)
{

}

int BassSampleChannel::Play()
{

	LOG(LogLevel::Debug) << "BassSampleChannel::Play() : add play action.";

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Debug) << "BassSampleChannel::Play() : create channel.";

		if (!GetIsLoaded()) {
			channelID = 0;
			return 0;
		}

		channelID = dynamic_cast<BassSample*>(sample)->CreateChannel();
		BASS_ChannelSetAttribute(channelID, BASS_ATTRIB_PAN, 0);
		BASS_ChannelSetAttribute(channelID, BASS_ATTRIB_VOL, volumeCalculated->GetValue());

		return 0;
	}, "Lambda_BassSampleChannel::CreateChannel");

	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Debug) << "BassSampleChannel::Play() : play channel.";

		BASS_ChannelPlay(channelID, false);

		return 0;
	}, "Lambda_BassSampleChannel::Play");
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
