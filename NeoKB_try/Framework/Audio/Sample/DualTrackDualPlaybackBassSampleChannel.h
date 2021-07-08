#ifndef DUAL_TRACK_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define DUAL_TRACK_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H


#include "DualPlaybackBassSampleChannel.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class DualTrackDualPlaybackBassSampleChannel : public DualPlaybackBassSampleChannel {


	public:

		DualTrackDualPlaybackBassSampleChannel(Sample* s);

		virtual int Update();

		virtual int Play();

		int SetTimbreRange(float start, float end);

	protected:

		/// <summary>
		/// 當音量超過0.3後，就開始將pan從-1往上調
		/// </summary>
		float gradientTimbreStartVolume = 0.3;

		/// <summary>
		/// 當音量超過0.3後，就開始將pan從-1往上調，直到音量為0.7時，pan調至1
		/// </summary>
		float gradientTimbreEndVolume = 0.7;


	};


}}}






#endif