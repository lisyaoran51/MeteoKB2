#ifndef REALTIME_REVERB_DUAL_TRACK_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define REALTIME_REVERB_DUAL_TRACK_DUAL_PLAYBACK_BASS_SAMPLE_CHANNEL_H

#include "DualTrackDualPlaybackBassSampleChannel.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class RealtimeReverbDualTrackDualPlaybackBassSampleChannel : public DualTrackDualPlaybackBassSampleChannel {


	public:

		RealtimeReverbDualTrackDualPlaybackBassSampleChannel(Sample* s);
		~RealtimeReverbDualTrackDualPlaybackBassSampleChannel();

		virtual int Play();

		virtual int FadeOut();

	protected:


		/// <summary>
		/// 用多個重疊的Channel來製造出效果
		/// </summary>
		int reverbChannelID[5];

		/// <summary>
		/// 前置delay不可高於0.1
		/// </summary>
		float reverbFadeoutTime = 1.f;

		/// <summary>
		/// 前置delay不可高於0.1
		/// </summary>
		float predelay = 0.1f;

		/// <summary>
		/// 每個reverb channel之間的間隔，0.3聽起來會比較糊，0.2比較清晰。
		/// </summary>
		float delays[5] = { 0, 0.03, 0.06, 0.09, 0.12 };

		/// <summary>
		/// 每個reverb channel的音量。
		/// </summary>
		float reverbVolumes[5] = { 0.08, 0.09, 0.1, 0.11, 0.12 };

	};


}}}











#endif