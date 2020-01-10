#ifndef MULTI_PLAYBACK_SAMPLE_CHANNEL_H
#define MULTI_PLAYBACK_SAMPLE_CHANNEL_H

#include "SampleChannel.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class MultiPlaybackSampleChannel : virtual public SampleChannel {

	public:

		MultiPlaybackSampleChannel(Sample* s, int pAmount);

		//virtual int Stop(int channelId); 不會有這個狀況，要停就是所有都停，所以先不寫

	protected:

		int playbackAmount = 0;

		int* channelIds = nullptr;

		virtual int getChannelToPlay() = 0;

		virtual int createSampleChannel() = 0;

	};


}}}



#endif