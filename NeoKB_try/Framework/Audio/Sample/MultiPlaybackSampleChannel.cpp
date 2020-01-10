#include "MultiPlaybackSampleChannel.h"


using namespace Framework::Audio::Samples;


MultiPlaybackSampleChannel::MultiPlaybackSampleChannel(Sample * s, int pAmount): SampleChannel(s)
{

	playbackAmount = pAmount;

	channelIds = new int[pAmount];

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		for (int i = 0; i < pAmount; i++) {

			channelIds[i] = createSampleChannel();

		}

		return 0;
	}, "Lambda_MultiPlaybackSampleChannel::CreateSampleChannels");

	

}
