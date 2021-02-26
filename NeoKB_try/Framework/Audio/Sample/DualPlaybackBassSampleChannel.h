#ifndef DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H

#include "../AudioCollectionManager.h"
#include "SampleChannel.h"
#include "../../../ThirdParty/Bass/bass.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class DualPlaybackBassSampleChannel : public SampleChannel {

		int channelID[2];

		int tempPlayingPlayback = 0;


	public:

		DualPlaybackBassSampleChannel(Sample* s);
		~DualPlaybackBassSampleChannel();

		virtual int Play();

		virtual int Play(double v);

		virtual int Stop();

		virtual int FadeOut();

		virtual int StopFadeOut();

		virtual bool GetIsPlaying();

		virtual bool GetIsLoaded();

		//virtual int OnStateChange();

	protected:

		double lastVolume = -1;
		



	};


}}}





#endif