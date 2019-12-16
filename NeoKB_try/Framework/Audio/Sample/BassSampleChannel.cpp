#include "BassSampleChannel.h"

#include "BassSample.h"

using namespace Framework::Audio::Samples;


BassSampleChannel::BassSampleChannel(Sample * s): SampleChannel(s)
{

}

int BassSampleChannel::Play()
{

	return 0;
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
		BASS_ChannelSetAttribute(channelID, BASS_ATTRIB_VOL, 1);

		return 0;
	}, "Lambda_BassSampleChannel::CreateChannel");

	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Debug) << "BassSampleChannel::Play() : play channel.";

		BASS_ChannelPlay(channelID, false);

		return 0;
	}, "Lambda_BassSampleChannel::Play");
	return 0;
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

bool BassSampleChannel::GetIsLoaded()
{
	return sample->GetIsLoaded();
}
