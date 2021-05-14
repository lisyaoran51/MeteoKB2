#ifndef TWO_STAGE_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define TWO_STAGE_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H

#include "../AudioCollectionManager.h"
#include "SampleChannel.h"
#include "../../../ThirdParty/Bass/bass.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class TwoStageDualPlaybackBassSampleChannel : public SampleChannel {

		int channelID[2];

		int tempPlayingPlayback = 0;

	public:

		TwoStageDualPlaybackBassSampleChannel(Sample* sA, Sample* sB, float sPoint, float tLength);
		virtual ~TwoStageDualPlaybackBassSampleChannel();

		virtual int Play();

		virtual int Play(double v);

		virtual int Stop();

		virtual int FadeOut();

		virtual int StopFadeOut();

		virtual bool GetIsPlaying();

		virtual bool GetIsLoaded();

		virtual int OnStateChange();

	protected:

		



	};


}}}





#endif