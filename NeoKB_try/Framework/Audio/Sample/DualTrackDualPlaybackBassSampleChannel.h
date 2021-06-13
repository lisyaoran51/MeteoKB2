#ifndef DUAL_TRACK_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define DUAL_TRACK_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H


#include "DualPlaybackBassSampleChannel.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class DualTrackDualPlaybackBassSampleChannel : public DualPlaybackBassSampleChannel {

		int channelID[2];

		int tempPlayingPlayback = 0;


	public:

		DualTrackDualPlaybackBassSampleChannel(Sample* s);

		virtual int Play();

	protected:

		/// <summary>
		/// 當音量超過0.3後，就開始將pan從-1往上調
		/// </summary>
		float startVolume = 0.3;

		/// <summary>
		/// 當音量超過0.3後，就開始將pan從-1往上調，直到音量為0.7時，pan調至1
		/// </summary>
		float endVolume = 0.7;


	};


}}}






#endif