#include "BassSampleChannel.h"

#include "BassSample.h"

using namespace Framework::Audio::Samples;


BassSampleChannel::BassSampleChannel(Sample * s): SampleChannel(s)
{

}

int BassSampleChannel::Play()
{
	pendingActions.Add(this, [=]() {

		if (!GetIsLoaded()) {
			channelID = 0;
			return 0;
		}

		channelID = dynamic_cast<BassSample*>(sample)->CreateChannel();
		BASS_ChannelSetAttribute(channelID, BASS_ATTRIB_PAN, 0);
		BASS_ChannelSetAttribute(channelID, BASS_ATTRIB_VOL, 1);

		return 0;
	}, "Lambda_BassSampleChannel::CreateChannel");

	pendingActions.Add(this, [=]() {
		
		BASS_ChannelPlay(channelID, false);
		return 0;
	}, "Lambda_BassSampleChannel::Play");
	return 0;
}

int BassSampleChannel::Stop()
{
	pendingActions.Add(this, [=]() {

		BASS_ChannelStop(channelID);
		return 0;
	}, "Lambda_BassSampleChannel::Stop");

	return 0;
}

bool BassSampleChannel::GetIsLoaded()
{
	return sample->GetIsLoaded();
}
