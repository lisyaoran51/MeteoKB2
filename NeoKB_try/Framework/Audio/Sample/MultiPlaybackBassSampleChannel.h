#ifndef MULTI_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define MULTI_PLAYBACK_BASS_SAMPLE_CHANNEL_H

#include "MultiPlaybackSampleChannel.h"
#include "../../../ThirdParty/Bass/bass.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class MultiPlaybackBassSampleChannel : public MultiPlaybackSampleChannel {

	public:

		MultiPlaybackBassSampleChannel(Sample* s, int pAmount, int tAmount, OverrideType oType);

		virtual int Play(int trackNumber);

		virtual int Play(int trackNumber, double v);

		virtual int Stop(int trackNumber);

		virtual int FadeOut();

		virtual int FadeOut(int trackNumber);

		virtual int StopFadeOut();

		virtual int StopFadeOut(int trackNumber);

		virtual bool GetIsPlaying();

		virtual bool GetIsPlaying(int trackNumber);

		virtual bool GetIsLoaded();

		virtual int OnStateChange(int trackNumber);

	protected:

		virtual int createSampleChannel();

		virtual int getChannelToPlay(int trackNumber);

	};


}}}



#endif