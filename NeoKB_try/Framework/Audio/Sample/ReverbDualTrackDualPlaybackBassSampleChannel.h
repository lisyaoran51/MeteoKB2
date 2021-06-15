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
		/// ���q�W�L0.3��A�N�}�l�Npan�q-1���W��
		/// </summary>
		int reverbChannelID[2];

		float reverbFadeoutTime = 1.f;



	};


}}}











#endif