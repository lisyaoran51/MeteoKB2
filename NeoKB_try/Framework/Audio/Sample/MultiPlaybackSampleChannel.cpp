#include "MultiPlaybackSampleChannel.h"


using namespace Framework::Audio::Samples;


MultiPlaybackSampleChannel::MultiPlaybackSampleChannel(Sample * s, int pAmount): SampleChannel(s)
{

	playbackAmount = pAmount;

	channelIds = new int[pAmount];

	for (int i = 0; i < pAmount; i++) {

		channelIds[i] = createSampleChannel();

	}

}
