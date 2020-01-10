#ifndef MULTI_PLAYBACK_SAMPLE_CHANNEL_H
#define MULTI_PLAYBACK_SAMPLE_CHANNEL_H

#include "SampleChannel.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class MultiPlaybackSampleChannel : virtual public SampleChannel {

	public:

		MultiPlaybackSampleChannel(Sample* s, int pAmount);

		//virtual int Stop(int channelId); ���|���o�Ӫ��p�A�n���N�O�Ҧ������A�ҥH�����g

	protected:

		int playbackAmount = 0;

		int* channelIds = nullptr;

		virtual int getChannelToPlay() = 0;

		virtual int createSampleChannel() = 0;

	};


}}}



#endif