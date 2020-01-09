#ifndef MULTI_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define MULTI_PLAYBACK_BASS_SAMPLE_CHANNEL_H

#include "SampleChannel.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class MultiPlaybackBassSampleChannel : virtual public SampleChannel {

	public:

		MultiPlaybackBassSampleChannel(Sample* s);



	};


}}}



#endif