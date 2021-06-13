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
		/// ���q�W�L0.3��A�N�}�l�Npan�q-1���W��
		/// </summary>
		float startVolume = 0.3;

		/// <summary>
		/// ���q�W�L0.3��A�N�}�l�Npan�q-1���W�աA���쭵�q��0.7�ɡApan�զ�1
		/// </summary>
		float endVolume = 0.7;


	};


}}}






#endif