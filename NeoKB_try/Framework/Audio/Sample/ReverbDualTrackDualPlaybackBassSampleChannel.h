#ifndef REVERB_DUAL_TRACK_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define REVERB_DUAL_TRACK_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H

#include "DualTrackDualPlaybackBassSampleChannel.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class ReverbDualTrackDualPlaybackBassSampleChannel : public DualTrackDualPlaybackBassSampleChannel {


	public:

		ReverbDualTrackDualPlaybackBassSampleChannel(Sample* s, Sample* rSample);
		~ReverbDualTrackDualPlaybackBassSampleChannel();

		virtual int Play();

		virtual int FadeOut();

	protected:

		Sample* reverbSample = nullptr;

		/// <summary>
		/// 當音量超過0.3後，就開始將pan從-1往上調
		/// </summary>
		int reverbChannelID[2];

		float reverbFadeoutTime = 1.f;



	};


}}}











#endif