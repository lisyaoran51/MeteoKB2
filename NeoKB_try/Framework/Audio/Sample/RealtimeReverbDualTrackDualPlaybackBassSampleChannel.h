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
		/// �Φh�ӭ��|��Channel�ӻs�y�X�ĪG
		/// </summary>
		int reverbChannelID[5];

		/// <summary>
		/// �e�mdelay���i����0.1
		/// </summary>
		float reverbFadeoutTime = 1.f;

		/// <summary>
		/// �e�mdelay���i����0.1
		/// </summary>
		float predelay = 0.1f;

		/// <summary>
		/// �C��reverb channel���������j�A0.3ť�_�ӷ|����k�A0.2����M���C
		/// </summary>
		float delays[5] = { 0, 0.03, 0.06, 0.09, 0.12 };

		/// <summary>
		/// �C��reverb channel�����q�C
		/// </summary>
		float reverbVolumes[5] = { 0.08, 0.09, 0.1, 0.11, 0.12 };

	};


}}}











#endif